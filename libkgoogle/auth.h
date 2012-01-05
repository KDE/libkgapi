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


#ifndef LIBKGOOGLE_AUTH_H
#define LIBKGOOGLE_AUTH_H

#include <exception>

#include <qobject.h>
#include <qwindowdefs.h>
#include <qstringlist.h>

#include <libkgoogle/libkgoogle_export.h>
#include <libkgoogle/common.h>
#include <libkgoogle/account.h>

namespace KWallet {
  class Wallet;
}

class QNetworkReply;

namespace KGoogle {

  /**
   * Auth provides API for authentication against Google services.
   *
   * The Auth object is a singleton.
   *
   * When created for the first time, the KWallet is opened asynchronously.
   * When the operation is finished, the signal KGoogle::Auth::ready() is
   * emitted. When Auth fails to open the KWallet, KGoogle::Auth::error() is
   * emitted respective error code.
   */
  class LIBKGOOGLE_EXPORT Auth: public QObject
  {

    Q_OBJECT

    public:
      /**
       * @return Returns single instance of KGoogle::Auth.
       */
      static Auth* instance();

      virtual ~Auth();

      /**
       * Retrieves \p account from KWallet.
       *
       * It is up to user to delete the Account when no longer needed.
       *
       * @param account Name of account whose authentication to obtain
       * @return Returns KGoogle::Account or NULL when no such account is found in the
       *         KWallet or access to KWallet failed.
       */
      const KGoogle::Account* getAccount(const QString &account);

      /**
       * Retrieves list of all accounts from KWallet.
       *
       * It's up to user to delete all accounts in the list when no longer needed.
       *
       * @return Returns list of all Google accounts from KWallet.
       */
      const QList< KGoogle::Account* > getAccounts();

      /**
       * Stores \p account in KWallet.
       *
       * When account with same name already exists in KWallet
       * it will be overwritten by the new \p account.
       *
       * @param account Account to store in KWallet
       */
      void storeAccount(const KGoogle::Account *account);

      /**
       * Authenticates \p account against Google services.
       *
       * When \p account contains no access or refresh token,
       * a complete authentication is performed (including dislaying)
       * KGoogle::AuthDialog.
       *
       * When the \p account has both, access and refresh token set then
       * the refresh token is used to only obtain a new access token.
       *
       * The operations runs asynchronously. When finished,
       * KGoogle::Auth::authenticated() signal is emitted.
       * Upon error, KGoogle::Auth::error() signal is emitted.
       *
       * @param account Account whose token is to be refreshed.
       * @param autoSave Whether KGoogle::Auth should automatically
       *                 call KGoogle::Auth::storeAccount() to save changes 
       *                 to KWallet.
       */
      void authenticate(KGoogle::Account *account, bool autoSave = true);

      /**
       * Revokes tokens for \p account and removes it from KWallet.
       *
       * This method will not throw any exception when the account
       * is invalid or does not exist, only when access to the secure
       * backend fails.
       *
       * @param account Account to which revoke access.
       * @return Returns \p true when account is succesfully removed, \p false
       *         when account does not exist or something fails.
       */
      bool revoke(KGoogle::Account *account);

    Q_SIGNALS:
      /**
       * Emitted when an error occurs
       *
       * Signalizes any authentication error or when communication
       * with KWallet fails.
       */
      void error(KGoogle::Error errCode, QString msg);

      /**
       * Account was succesfully (re-)authenticated.
       * 
       * This signal is emitted when \p account was succesfully authenticated
       * (for the first time), or when just tokens were refreshed.
       */
      void authenticated(KGoogle::Account *account);

    private Q_SLOTS:
      void fullAuthentication(KGoogle::Account *account, bool autoSave);
      void fullAuthenticationFinished(KGoogle::Account *account);

      void refreshTokens(KGoogle::Account *account, bool autoSave);
      void refreshTokensFinished(QNetworkReply *reply);

    private:
      Auth();
      Auth(const Auth& other);
      Auth& operator=(const Auth &other);

      KWallet::Wallet *m_kwallet;

      static Auth *m_instance;
  };

} // namespace KGoogle

#endif // LIBKGOOGLE_AUTH_H
