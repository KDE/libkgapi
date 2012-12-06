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

#include "auth.h"
#include "auth_p.h"
#include "common.h"
#include "services/accountinfo.h"
#include "ui/authwidget.h"

#include <KWallet/Wallet>
#include <KLocalizedString>
#include <KDialog>
#include <KWindowSystem>

#include <QtCore/QMap>
#include <QtCore/QMutex>

using namespace KWallet;
using namespace KGAPI;

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

    d->kwalletFolder = QLatin1String("libkgapi");
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

QString Auth::apiKey() const
{
    return d_func()->apiKey;
}

QString Auth::apiSecret() const
{
    return d_func()->apiSecret;
}

KGAPI::Account::Ptr Auth::getAccount(const QString &account)
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

QList< KGAPI::Account::Ptr > Auth::getAccounts()
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
    Q_FOREACH(const QString &accName, list) {

        QMap< QString, QString > map;
        d->kwallet->readMap(accName, map);

        /* Make sure we ignore authentication entries from libkgoogle 0.2 */
        if (!map.contains(QLatin1String("accessToken"))
            || !map.contains(QLatin1String("refreshToken"))
            || !map.contains(QLatin1String("scopes"))) {

            continue;
        }

        /* If this account was already added to the list fom cache,
         * then don't insert it again! */
        if (d->accountsCache.contains(accName)) {
            continue;
        }

        QStringList scopes = map.value(QLatin1String("scopes")).split(QLatin1Char(','));
        QList< QUrl > scopeUrls;
        Q_FOREACH(const QString & scope, scopes) {
            scopeUrls << QUrl(scope);
        }

        Account::Ptr account(new Account(accName, map.value(QLatin1String("accessToken")),
					 map.value(QLatin1String("refreshToken")), scopeUrls));

        accounts.append(account);
        d->accountsCache.insert(account->accountName(), account);
    }

    return accounts;
}

void Auth::storeAccount(const KGAPI::Account::Ptr &account)
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
    Q_FOREACH(const QUrl & scope, account->scopes()) {
        scopes << scope.toString();
    }

    QMap< QString, QString > map;
    map.insert(QLatin1String("accessToken"), account->accessToken());
    map.insert(QLatin1String("refreshToken"), account->refreshToken());
    map.insert(QLatin1String("scopes"), scopes.join(QLatin1String(",")));
    d->kwallet->writeMap(account->accountName(), map);

    /* If the account is not yet in cache then put it in there. Otherwise
     * the @param account is already a copy of an account that's in cache
     * and thus it has already been updated globally */
    if (!d->accountsCache.contains(account->accountName())) {
        d->accountsCache.insert(account->accountName(), account);
    }
}

void Auth::authenticate(KGAPI::Account::Ptr &account, bool autoSave)
{
    Q_D(Auth);

    Ui::AuthWidget *widget;
    KDialog *dlg;

    widget = d->authenticate(account, autoSave);

    if (widget) {
        dlg = new KDialog();
        dlg->setModal(true);
        KWindowSystem::setMainWindow(dlg, KWindowSystem::activeWindow());

        dlg->setMainWidget(widget);
        connect(widget, SIGNAL(error(KGAPI::Error,QString)),
                this, SIGNAL(error(KGAPI::Error,QString)));
        connect(dlg, SIGNAL(cancelClicked()), dlg, SLOT(delayedDestruct()));

        if (d->dialogAutoClose) {
            connect(widget, SIGNAL(authenticated(KGAPI::Account::Ptr&)),
                    dlg, SLOT(delayedDestruct()));
            connect(widget, SIGNAL(error(KGAPI::Error,QString)),
                    dlg, SLOT(delayedDestruct()));
        }

        dlg->show();
        dlg->setButtons(KDialog::Cancel);
        widget->authenticate();
    }
}


KGAPI::Ui::AuthWidget* Auth::authenticateWithWidget(Account::Ptr& account, bool autoSave)
{
    Q_D(Auth);

    return d->authenticate(account, autoSave);
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

            account->setAccessToken(QLatin1String(""));
            account->setRefreshToken(QLatin1String(""));
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

void Auth::setUsername(const QString& username)
{
    Q_D(Auth);
    d->username = username;
}

void Auth::setPassword(const QString& password)
{
    Q_D(Auth);
    d->password = password;
}

void Auth::setDialogAutoClose(bool close)
{
    Q_D(Auth);
    d->dialogAutoClose = close;
}

void Auth::clearCredentials()
{
    Q_D(Auth);
    d->username.clear();
    d->password.clear();
}
