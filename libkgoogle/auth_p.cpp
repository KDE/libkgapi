/*
    libKGoogle - Auth
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


#include "auth_p.h"

#include <kwallet.h>
#include <kwindowsystem.h>
#include <kio/accessmanager.h>
#include <kdebug.h>

#include <qjson/parser.h>

#include <qnetworkreply.h>
#include <qnetworkrequest.h>

#include "auth.h"
#include "authdialog.h"

using namespace KGoogle;
using namespace KWallet;

AuthPrivate::AuthPrivate(KGoogle::Auth* const parent):
  QObject(),
  kwalletFolder("libkgoogle"),
  kwallet(0),
  q_ptr(parent)
{ }

AuthPrivate::~AuthPrivate()
{
  if (kwallet) {
    kwallet->closeWallet(Wallet::NetworkWallet(), false);
    delete kwallet;
  }
}

void AuthPrivate::fullAuthentication (KGoogle::Account *account, bool autoSave)
{
  Q_Q(Auth);

  AuthDialog *dlg = new AuthDialog(KWindowSystem::activeWindow());
  dlg->setProperty("autoSaveAccount", QVariant(autoSave));

  connect(dlg, SIGNAL(error(KGoogle::Error,QString)),
          q, SIGNAL(error(KGoogle::Error,QString)));
  connect(dlg, SIGNAL(authenticated(KGoogle::Account*)),
          this, SLOT(fullAuthenticationFinished(KGoogle::Account*)));
  connect(dlg, SIGNAL(accepted()),
          dlg, SLOT(deleteLater()));

  dlg->show();
  dlg->authenticate(account);
}

void AuthPrivate::fullAuthenticationFinished (KGoogle::Account *account)
{
  Q_Q(Auth);

  bool autoSave = false;

  /* Actually this slot shouldn't be invoked by anything else but
   * AuthDialog, but just to make sure... */
  if (sender() && qobject_cast< AuthDialog* >(sender()))
    autoSave = sender()->property("autoSaveAccount").toBool();

  if (autoSave) {
    try {
      q->storeAccount(account);
    } catch (Exception::BaseException &e) {
      emit q->error(e.code(), e.what());
      return;
    }
  }

  /* Reset scopesChanged flag */
  account->m_scopesChanged = false;

  emit q->authenticated(account);
}


void AuthPrivate::refreshTokens (KGoogle::Account *account, bool autoSave)
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


void AuthPrivate::refreshTokensFinished (QNetworkReply *reply)
{
  Q_Q(Auth);

  if (reply->error()) {
    emit q->error(NetworkError, reply->errorString());
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
    emit q->error(InvalidResponse, i18n("Failed to parse newly fetched tokens"));
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
    q->storeAccount(account);

  emit q->authenticated(account);
}
