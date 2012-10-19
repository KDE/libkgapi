/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

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
#include "ui/authwidget.h"
#include "services/accountinfo.h"

using namespace KGAPI;
using namespace KWallet;

AuthPrivate::AuthPrivate(KGAPI::Auth* const parent):
    QObject(),
    kwalletFolder("libkgapi"),
    kwallet(0),
    dialogAutoClose(true),
    q_ptr(parent)
{ }

AuthPrivate::~AuthPrivate()
{
    if (kwallet) {
        kwallet->closeWallet(Wallet::NetworkWallet(), false);
        delete kwallet;
    }
}

bool AuthPrivate::initKWallet()
{
    if (kwallet && kwallet->isOpen())
        return true;

    if (kwallet)
        disconnect(kwallet, SIGNAL(folderUpdated(QString)));

    kwallet = Wallet::openWallet(Wallet::NetworkWallet(), 0, Wallet::Synchronous);
    if (!kwallet || !kwallet->isOpen()) {
        throw Exception::BackendNotReady();
        return false;
    }
    connect(kwallet, SIGNAL(folderUpdated(QString)),
            this, SLOT(kwalletFolderChanged(QString)));

    return true;
}

Account::Ptr AuthPrivate::getAccountFromWallet(const QString& account)
{
    QMap< QString, QString > map;
    if (kwallet->readMap(account, map) != 0) {
        throw Exception::UnknownAccount(account);
        return Account::Ptr();
    }

    QStringList scopes = map["scopes"].split(',');
    QList< QUrl > scopeUrls;
    Q_FOREACH(const QString & scope, scopes) {
        scopeUrls << QUrl(scope);
    }

    /* Create new item from data in KWallet and store it in cache before returning it
     * to user */
    return Account::Ptr(new Account(account, map["accessToken"], map["refreshToken"], scopeUrls));
}

KGAPI::Ui::AuthWidget* AuthPrivate::authenticate(Account::Ptr& account, bool autoSave)
{
    if (!initKWallet()) {
        return 0;
    }

    if (account.isNull()) {
        throw Exception::InvalidAccount();
        return 0;
    }

    if (account->refreshToken().isEmpty() || (account->m_scopesChanged == true)) {

        account->addScope(Services::AccountInfo::EmailScopeUrl);

        /* Pre-fill the username in the dialog so that user knows what account
         * (s)he is re-authenticating for */
        if (!account->accountName().isEmpty() && username.isEmpty())
            username = account->accountName();

        return fullAuthentication(account, autoSave);

    } else {

        if (account->accountName().isEmpty()) {
            throw Exception::InvalidAccount();
            return 0;
        }

        refreshTokens(account, autoSave);
        return 0;
    }
}

KGAPI::Ui::AuthWidget* AuthPrivate::fullAuthentication(KGAPI::Account::Ptr &account, bool autoSave)
{
    Q_Q(Auth);

    Ui::AuthWidget *widget =  new Ui::AuthWidget();
    widget->setProperty("autoSaveAccount", QVariant(autoSave));

    connect(widget, SIGNAL(error(KGAPI::Error,QString)),
            q, SIGNAL(error(KGAPI::Error,QString)));
    connect(widget, SIGNAL(authenticated(KGAPI::Account::Ptr&)),
            this, SLOT(fullAuthenticationFinished(KGAPI::Account::Ptr&)));

    widget->setUsername(username);
    widget->setPassword(password);
    widget->setAccount(account);

    return widget;
}

void AuthPrivate::fullAuthenticationFinished(KGAPI::Account::Ptr &account)
{
    Q_Q(Auth);

    bool autoSave = false;

    /* Actually this slot shouldn't be invoked by anything else but
     * AuthDialog, but just to make sure... */
    if (sender() && qobject_cast< Ui::AuthWidget* >(sender())) {
        autoSave = sender()->property("autoSaveAccount").toBool();
    }

    if (autoSave) {
        try {
            q->storeAccount(account);
        } catch (Exception::BaseException &e) {
            Q_EMIT q->error(e.code(), e.what());
            return;
        }
    }

    /* Reset scopesChanged flag */
    account->m_scopesChanged = false;

    Q_EMIT q->authenticated(account);
}

void AuthPrivate::authDialogCancelled()
{
    Q_Q(Auth);
    Q_EMIT q->error(AuthCancelled, i18n("Authentication cancelled"));
}


void AuthPrivate::refreshTokens(KGAPI::Account::Ptr &account, bool autoSave)
{
    QNetworkAccessManager *nam = new KIO::Integration::AccessManager(this);
    nam->setCookieJar(new QNetworkCookieJar());
    QNetworkRequest request;

    connect(nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(refreshTokensFinished(QNetworkReply*)));
    connect(nam, SIGNAL(finished(QNetworkReply*)),
            nam, SLOT(deleteLater()));

    request.setUrl(QUrl("https://accounts.google.com/o/oauth2/token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setAttribute(QNetworkRequest::User, QVariant(autoSave));
    request.setAttribute(QNetworkRequest::UserMax, qVariantFromValue< Account::Ptr >(account));

    QUrl params;
    params.addQueryItem("client_id", apiKey);
    params.addQueryItem("client_secret", apiSecret);
    params.addQueryItem("refresh_token", account->refreshToken());
    params.addQueryItem("grant_type", "refresh_token");

#ifdef DEBUG_RAWDATA
    kDebug() << "Requesting token refresh: " << params.encodedQuery();
#endif

    nam->post(request, params.encodedQuery());
}


void AuthPrivate::refreshTokensFinished(QNetworkReply *reply)
{
    Q_Q(Auth);

    if (reply->error()) {
        Q_EMIT q->error(NetworkError, reply->errorString());
        return;
    }

    QNetworkRequest request = reply->request();
    bool autoSave = request.attribute(QNetworkRequest::User).toBool();
    Account::Ptr account = qVariantValue< Account::Ptr >(request.attribute(QNetworkRequest::UserMax));

    QByteArray rawData = reply->readAll();
    QJson::Parser parser;

    bool ok = true;
    QVariantMap map = parser.parse(rawData, &ok).toMap();

    if (!ok) {
        Q_EMIT q->error(InvalidResponse, i18n("Failed to parse newly fetched tokens"));
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

    if (autoSave) {
        try {
            q->storeAccount(account);
        } catch (Exception::BaseException &e) {
            Q_EMIT q->error(e.code(), e.what());
	    return;
        }
    }

    Q_EMIT q->authenticated(account);
}

void AuthPrivate::kwalletFolderChanged (const QString& folder)
{
    if (folder != kwalletFolder)
        return;

    kDebug() << "KWallet folder" << folder << "changed, checking if any of" << accountsCache.size()<< "cached accounts needs updating";

    /* Go through all cached accounts (this assumes the cache will be usually
     * really small - 1 or 2 accounts) and update changed values.
     *
     * We can't copy the new account over the old because that would not update
     * the value in all other copies of the cachedAcc object. */
    QMap< QString, Account::Ptr >::const_iterator iter = accountsCache.begin();
    while (iter != accountsCache.end()) {

        Account::Ptr walletAcc = getAccountFromWallet(iter.key());
        Account::Ptr cachedAcc = iter.value();

        if (walletAcc->accessToken() != cachedAcc->accessToken()) {
            cachedAcc->setAccessToken(walletAcc->accessToken());
        }

        if (walletAcc->refreshToken() != cachedAcc->refreshToken()) {
            cachedAcc->setRefreshToken(walletAcc->refreshToken());
        }

        if (walletAcc->scopes() != cachedAcc->scopes()) {
            cachedAcc->setScopes(walletAcc->scopes());
            cachedAcc->m_scopesChanged = false;
        }

        ++iter;
    }
}

