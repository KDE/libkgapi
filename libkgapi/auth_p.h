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

#ifndef LIBKGAPI_AUTH_P_H
#define LIBKGAPI_AUTH_P_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include "account.h"

namespace KWallet
{
class Wallet;
}

class QNetworkReply;

namespace KGAPI
{

class Auth;

namespace Ui {
class AuthWidget;
}

/**
 * \internal
 */
class AuthPrivate: public QObject
{

    Q_OBJECT

  public:
    explicit AuthPrivate(Auth* const parent);
    virtual ~AuthPrivate();

    QString kwalletFolder;
    KWallet::Wallet *kwallet;

    static KGAPI::Auth *instance;

    QMap< QString, Account::Ptr > accountsCache;

    QString apiKey;
    QString apiSecret;
    QString username;
    QString password;
    bool dialogAutoClose;

    bool initKWallet();

    Account::Ptr getAccountFromWallet(const QString &account);

  public Q_SLOTS:
    KGAPI::Ui::AuthWidget* authenticate(KGAPI::Account::Ptr &account, bool autoSave);
    void refreshTokens(KGAPI::Account::Ptr &account, bool autoSave);

  private Q_SLOTS:
    void authDialogCancelled();
    void refreshTokensFinished(QNetworkReply *reply);
    void kwalletFolderChanged(const QString &folder);
    void fullAuthenticationFinished(KGAPI::Account::Ptr &account);

  private:
    KGAPI::Ui::AuthWidget* fullAuthentication(KGAPI::Account::Ptr &account, bool autoSave);

    Auth* const q_ptr;
    Q_DECLARE_PUBLIC(Auth)
};

} /* namespace KGAPI */

#endif /* LIBKGAPI_AUTH_P_H */
