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
#include "auth_p.h"

#include "common.h"
#include "services/accountinfo.h"

#include <KDE/KWallet/Wallet>
#include <KDE/KLocalizedString>

#include <QtCore/QMap>
#include <QtCore/QMutex>

using namespace KWallet;
using namespace KGoogle;

Auth* AuthPrivate::instance = 0;

Auth *Auth::instance()
{
    static QMutex mutex;
    if (!AuthPrivate::instance) {
        mutex.lock();

        if (!AuthPrivate::instance)
            AuthPrivate::instance = new Auth();

        mutex.unlock();
    }

    return AuthPrivate::instance;
}

Auth::Auth():
    d_ptr(new AuthPrivate(this))
{
    Q_D(Auth);

    d->kwalletFolder = "libkgoogle";
    d->kwallet = 0;
}


Auth::~Auth()
{
    delete d_ptr;
}

void Auth::init (const QString& folder, const QString& apiKey, const QString& apiSecret)
{
    Q_D(Auth);

    d->kwalletFolder = folder;
    d->apiKey = apiKey;
    d->apiSecret = apiSecret;

}

void Auth::setKWalletFolder(const QString& folder)
{
    Q_D(Auth);

    d->kwalletFolder = folder;
}

QString Auth::kwalletFolder() const
{
    return d_func()->kwalletFolder;
}

const QString& Auth::apiKey() const
{
    return d_func()->apiKey;
}

const QString& Auth::apiSecret() const
{
    return d_func()->apiSecret;
}

KGoogle::Account::Ptr Auth::getAccount(const QString &account)
{
    Q_D(Auth);

    /* First try to lookup the account in cache */
    if (d->accountsCache.contains(account)) {
        return d->accountsCache.value(account);
    }

    if (!d->initKWallet())
        return Account::Ptr();

    if (!d->kwallet->hasFolder(d->kwalletFolder)) {
        throw Exception::UnknownAccount(account);
        return Account::Ptr();
    }

    d->kwallet->setFolder(d->kwalletFolder);

    Account::Ptr acc = d->getAccountFromWallet(account);
    d->accountsCache.insert(account, acc);

    return acc;
}

QList< KGoogle::Account::Ptr > Auth::getAccounts()
{
    Q_D(Auth);

    /* Initially get all cached accounts */
    QList< Account::Ptr > accounts;
    accounts = d->accountsCache.values();

    if (!d->initKWallet()) {
        return QList< Account::Ptr >();
    }

    if (!d->kwallet->hasFolder(d->kwalletFolder)) {
        return QList< Account::Ptr >();
    }

    d->kwallet->setFolder(d->kwalletFolder);
    QStringList list = d->kwallet->entryList();
    foreach(QString accName, list) {

        QMap< QString, QString > map;
        d->kwallet->readMap(accName, map);

        /* Make sure we ignore authentication entries from libkgoogle 0.2 */
        if (!map.contains("accessToken")
            || !map.contains("refreshToken")
            || !map.contains("scopes")) {

            continue;
        }

        /* If this account was already added to the list fom cache,
         * then don't insert it again! */
        if (d->accountsCache.contains(accName)) {
            continue;
        }

        QStringList scopes = map["scopes"].split(',');
        QList< QUrl > scopeUrls;
        foreach(const QString & scope, scopes) {
            scopeUrls << QUrl(scope);
        }

        Account::Ptr account(new Account(accName, map["accessToken"], map["refreshToken"], scopeUrls));

        accounts.append(account);
        d->accountsCache.insert(account->accountName(), account);
    }

    return accounts;
}

void Auth::storeAccount(const KGoogle::Account::Ptr &account)
{
    Q_D(Auth);

    if (!d->initKWallet())
        return;

    if (account.isNull() || account->accountName().isEmpty()
        || account->accessToken().isEmpty() || account->refreshToken().isEmpty()) {

        throw Exception::InvalidAccount();
        return;
    }

    if (!d->kwallet->hasFolder(d->kwalletFolder))
        d->kwallet->createFolder(d->kwalletFolder);

    d->kwallet->setFolder(d->kwalletFolder);

    if (d->kwallet->hasEntry(account->accountName()))
        d->kwallet->removeEntry(account->accountName());

    QStringList scopes;
    foreach(const QUrl & scope, account->scopes()) {
        scopes << scope.toString();
    }

    QMap< QString, QString > map;
    map["accessToken"] = account->accessToken();
    map["refreshToken"] = account->refreshToken();
    map["scopes"] = scopes.join(",");
    d->kwallet->writeMap(account->accountName(), map);

    /* If the account is not yet in cache then put it in there. Otherwise
     * the @param account is already a copy of an account that's in cache
     * and thus it has already been updated globally */
    if (!d->accountsCache.contains(account->accountName())) {
        d->accountsCache.insert(account->accountName(), account);
    }
}

void Auth::authenticate(KGoogle::Account::Ptr &account, bool autoSave)
{
    Q_D(Auth);

    if (!d->initKWallet()) {
        return;
    }

    if (account.isNull()) {
        throw Exception::InvalidAccount();
        return;
    }

    if (account->refreshToken().isEmpty() || (account->m_scopesChanged == true)) {

        account->addScope(Services::AccountInfo::EmailScopeUrl);
        d->fullAuthentication(account, autoSave);

    } else {

        if (account->accountName().isEmpty()) {
            throw Exception::InvalidAccount();
            return;
        }

        d->refreshTokens(account, autoSave);

    }
}

bool Auth::revoke(Account::Ptr &account)
{
    Q_D(Auth);

    if (account.isNull() || account->accountName().isEmpty()) {
        return false;
    }

    if (!d->initKWallet())
        return false;

    if (!d->kwallet->hasFolder(d->kwalletFolder))
        return false;

    if (d->kwallet->hasEntry(account->accountName())) {

        if (d->kwallet->removeEntry(account->accountName()) == 0) {

            account->setAccessToken("");
            account->setRefreshToken("");
            account->setScopes(QList< QUrl >());

            if (d->accountsCache.contains(account->accountName())) {
                d->accountsCache.remove(account->accountName());
            }

            return true;

        } else {
            return false;
        }

    } else {
        return false;
    }
}
