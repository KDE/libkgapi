/*
    libKGoogle - KGoogleAuth
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "auth.h"
#include "authdialog.h"
#include "common.h"

#include <kwallet.h>
#include <kdebug.h>
#include <kio/accessmanager.h>
#include <klocalizedstring.h>
#include <kwindowsystem.h>

#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qmutex.h>

#include <qjson/parser.h>

using namespace KWallet;
using namespace KGoogle;

#define KWALLET_FOLDER "Akonadi Google"

Auth* Auth::m_instance = 0;

Auth *Auth::instance()
{
  static QMutex mutex;
  if (!m_instance) {
    mutex.lock();

    if (!m_instance)
      m_instance = new Auth();

    mutex.unlock();
  }

  return m_instance;
}

Auth::Auth()
{
  m_kwallet = Wallet::openWallet(Wallet::NetworkWallet(), 0, Wallet::Asynchronous);
  connect(m_kwallet, SIGNAL(walletOpened(bool)),
	  this, SLOT(walletOpened(bool)));
}


Auth::~Auth()
{
  if (m_kwallet) {
    m_kwallet->closeWallet(Wallet::NetworkWallet(), false);
    delete m_kwallet;
  }
}

const KGoogle::Account *Auth::getAccount (const QString &account)
{
  if (!m_kwallet->isOpen()) {
    emit error(KGoogle::BackendNotReady, i18n("KWallet is not opened"));
    throw Exception::BackendNotReady();
    return 0;
  }

  if (!m_kwallet->hasFolder(KWALLET_FOLDER)) {
    emit error(KGoogle::UnknownAccount, i18n("Account %1 does not exist", account));
    throw Exception::UnknownAccount(account);
    return 0;
  }

  m_kwallet->setFolder(KWALLET_FOLDER);

  QMap< QString, QString > map;
  if (m_kwallet->readMap(account, map) != 0) {
    emit error(KGoogle::UnknownAccount, i18n("Account %1 does not exist", account));
    throw Exception::UnknownAccount(account);
    return 0;
  }

  Account *acc = new Account(account, map["accessToken"], map["refreshToken"], map["scopes"].split(','));
  return acc;
}

const QList< const KGoogle::Account * > Auth::getAccounts()
{
  if (!m_kwallet->isOpen()) {
    emit error(KGoogle::BackendNotReady, i18n("KWallet is not opened"));
    throw Exception::BackendNotReady();
    return QList< const Account *>();
  }

  if (!m_kwallet->hasFolder(KWALLET_FOLDER))
    return QList< const Account *>();


  m_kwallet->setFolder(KWALLET_FOLDER);
  QStringList list = m_kwallet->entryList();
  QList< const Account * > accounts;
  foreach (QString accName, list) {
    QMap< QString, QString > map;
    m_kwallet->readMap(accName, map);
    accounts.append(new Account(accName, map["accessToken"], map["refreshToken"], map["scopes"].split(',')));
  }

  return accounts;
}

void Auth::storeAccount (const KGoogle::Account *account)
{
  if (!m_kwallet->isOpen()) {
    emit error(KGoogle::BackendNotReady, i18n("Kwallet is not opened"));
    throw Exception::BackendNotReady();
    return;
  }

  if (!m_kwallet->hasFolder(KWALLET_FOLDER))
    m_kwallet->createFolder(KWALLET_FOLDER);

  if (m_kwallet->hasEntry(account->accountName()))
    m_kwallet->removeEntry(account->accountName());

  QMap< QString, QString > map;
  map["accessToken"] = account->accessToken();
  map["refreshToken"] = account->refreshToken();
  map["scopes"] = account->scopes().join(",");
  m_kwallet->writeMap(account->accountName(), map);
}

void Auth::authenticate (KGoogle::Account *account, bool autoSave)
{
  if (!account || account->accountName().isEmpty()) {
    throw Exception::InvalidAccount();
    return;
  }

  if (account->refreshToken().isEmpty() || (account->m_scopesChanged == true)) {
    fullAuthentication(account, autoSave);
  } else {
    refreshTokens(account, autoSave);
  }
}

/******************************** PRIVATE METHODS ***************************/
void Auth::fullAuthentication (KGoogle::Account *account, bool autoSave)
{
  AuthDialog *dlg = new AuthDialog(KWindowSystem::activeWindow());
  dlg->setProperty("autoSaveAccount", QVariant(autoSave));

  connect(dlg, SIGNAL(error(KGoogle::Error,QString)),
          this, SIGNAL(error(KGoogle::Error,QString&)));
  connect(dlg, SIGNAL(authenticated(KGoogle::Account*)),
          this, SLOT(fullAuthenticationFinished(KGoogle::Account*)));
  connect(dlg, SIGNAL(accepted()),
          dlg, SLOT(deleteLater()));

  dlg->authenticate(account);
}

void Auth::fullAuthenticationFinished (KGoogle::Account *account)
{
  bool autoSave = false;

  /* Actually this slot shouldn't be invoked by anything else but
   * AuthDialog, but just to make sure... */
  if (sender() && qobject_cast<AuthDialog*>(sender()))
    autoSave = sender()->property("autoSaveAccount").toBool();

  if (autoSave)
    storeAccount(account);

  /* Reset scopesChanged flag */
  account->m_scopesChanged = false;

  emit authenticated(account);
}


void Auth::refreshTokens (KGoogle::Account *account, bool autoSave)
{
  QNetworkAccessManager *nam = new KIO::Integration::AccessManager(this);
  QNetworkRequest request;

  connect(nam, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(refreshTokensFinished(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
          nam, SLOT(deleteLater()));

  request.setUrl(QUrl("https://accounts.google.com/o/oauth2/token"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  request.setAttribute(QNetworkRequest::User, QVariant(autoSave));
  request.setAttribute(QNetworkRequest::UserMax, qVariantFromValue<Account*>(account));

  QUrl params;
  params.addQueryItem("client_id", APIClientID);
  params.addQueryItem("client_secret", APIClientSecret);
  params.addQueryItem("refresh_token", account->refreshToken());
  params.addQueryItem("grant_type", "refresh_token");

#ifdef DEBUG_RAWDATA
  kDebug() << "Requesting token refresh: " << params.encodedQuery();
#endif

  nam->post(request, params.encodedQuery());
}


void Auth::refreshTokensFinished (QNetworkReply *reply)
{
  if (reply->error()) {
    emit error(NetworkError, reply->errorString());
    return;
  }

  QNetworkRequest request = reply->request();
  bool autoSave = request.attribute(QNetworkRequest::User).toBool();
  Account *account = qVariantValue<Account*>(request.attribute(QNetworkRequest::UserMax));

  QByteArray rawData = reply->readAll();
  QJson::Parser parser;

  bool ok = true;
  QVariantMap map = parser.parse(rawData, &ok).toMap();

  if (!ok) {
    emit error(InvalidResponse, i18n("Failed to parse newly fetched tokens"));
    return;
  }

  /* Expected structure:
   * {
   *  "access_token": "the_access_token",
   *  "token_type":"Bearer",
   *  "expires_in":3600
   * }
   */

  account->setAccessToken(map["access_token"].toString());

  if (autoSave)
    storeAccount(account);

  emit authenticated(account);
}

void Auth::walletOpened (const bool success)
{
  if (!success)
    emit error(BackendNotReady, i18n("Failed to open KWallet"));
}
