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
    foreach(const QString & scope, scopes) {
        scopeUrls << QUrl(scope);
    }

    /* Create new item from data in KWallet and store it in cache before returning it
     * to user */
    return Account::Ptr(new Account(account, map["accessToken"], map["refreshToken"], scopeUrls));
}



void AuthPrivate::fullAuthentication(KGoogle::Account::Ptr &account, bool autoSave)
{
    Q_Q(Auth);

    AuthDialog *dlg = new AuthDialog(KWindowSystem::activeWindow());
    dlg->setProperty("autoSaveAccount", QVariant(autoSave));

    connect(dlg, SIGNAL(error(KGoogle::Error, QString)),
            q, SIGNAL(error(KGoogle::Error, QString)));
    connect(dlg, SIGNAL(authenticated(KGoogle::Account::Ptr&)),
            this, SLOT(fullAuthenticationFinished(KGoogle::Account::Ptr&)));
    connect(dlg, SIGNAL(accepted()),
            dlg, SLOT(deleteLater()));
    connect(dlg, SIGNAL(cancelClicked()),
            SLOT(authDialogCancelled()));

    dlg->setUsername(username);
    dlg->setPassword(password);
    dlg->show();
    dlg->authenticate(account);
}

void AuthPrivate::fullAuthenticationFinished(KGoogle::Account::Ptr &account)
{
    Q_Q(Auth);

    bool autoSave = false;

    /* Actually this slot shouldn't be invoked by anything else but
     * AuthDialog, but just to make sure... */
    if (sender() && qobject_cast< AuthDialog* >(sender())) {
        autoSave = sender()->property("autoSaveAccount").toBool();
    }

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

void AuthPrivate::authDialogCancelled()
{
    Q_Q(Auth);
    q->error(AuthCancelled, i18n("Authentication cancelled"));
}


void AuthPrivate::refreshTokens(KGoogle::Account::Ptr &account, bool autoSave)
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
        emit q->error(NetworkError, reply->errorString());
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

    if (autoSave) {
        q->storeAccount(account);
    }

    emit q->authenticated(account);
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
    foreach (const QString &accName, accountsCache.keys()) {

        Account::Ptr walletAcc = getAccountFromWallet(accName);
        Account::Ptr cachedAcc = accountsCache.value(accName);

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
    }
}

