/*
    libKGoogle - Account
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


#ifndef LIBKGOOGLE_ACCOUNT_H
#define LIBKGOOGLE_ACCOUNT_H

#include <qstring.h>
#include <qstringlist.h>
#include <qmetatype.h>

#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {

  /**
   * Account class represents a single Google account.
   *
   * Note that any change in the Account (new access token, new refresh token,
   * changed scopes...), must be stored in the KWallet via
   * KGoogle::Auth::storeAccount().
   *
   * Any change to scopes (adding or removing) requires passing the Account object
   * to KGoogle::Auth::authenticate() to retrieve new tokens that would grant access
   * to the new set of scopes only
   */
  class LIBKGOOGLE_EXPORT Account
  {

    /**
    * Grants access of KGoogle::Auth to KGoogle::Account::m_scopesChanged
    */
    friend class Auth;

    public:
      /**
       * Creates any empty account.
       *
       * Such object represents invalid account.
       */
      Account() { };

      /**
       * Constructs a new account
       *
       * @param account Google account name (usually user.name@gmail.com)
       * @param accessToken Access token to \p scopes for \p account
       * @param refreshToken Refres token
       * @param scopes List of scopes
       */
      Account(const QString &account, const QString &accessToken = QString(),
              const QString &refreshToken = QString(),
              const QStringList &scopes = QStringList());

      virtual ~Account();

      /**
       * @return Returns account name (user.name@gmail.com).
       */
      QString accountName() const;

      /**
       * Sets name of account.
       *
       * @param accountName Account name.
       */
      void setAccountName(const QString &accountName);

      /**
       * @return Returns access token for the account.
       */
      const QString& accessToken() const;

      /**
       * Sets a new access token for account.
       *
       * @param accessToken The new access token.
       */
      void setAccessToken(const QString &accessToken);

      /**
       * @return Returns refresh token.
       */
      const QString& refreshToken() const;

      /**
       * Sets a new refresh token for the access token.
       *
       * @param refreshToken The new refresh token.
       */
      void setRefreshToken(const QString &refreshToken);

      /**
       * @return Returns list of scopes the account is authenticated against.
       */
      const QStringList &scopes() const;

      /**
       * Sets new scopes. Use must re-authenticate to allow access to the new scopes.
       *
       * @note Note that user must re-authenticate via KGoogle::Auth::authenticate() to
       * receive a new token that will grant access to the new list of scopes.
       *
       * @param scopes List of the new scopes.
       */
      void setScopes(const QStringList &scopes);

      /**
       * Adds a single scope to account scopes.
       *
       * @note Note that user must re-authenticate in KGoogle::Auth::authenticate() to
       * receive a new token that will grant access to the new list of scopes.
       *
       * @param scope The new scope to be added.
       */
      void addScope(const QString &scope);

      /**
       * Removes scope from the list.
       *
       * @note Note that user must re-authenticate KGoogle::Auth::authenticate() to
       * receive a new token that will grant access only to the new subset of scopes.
       *
       * @param scope The scope to be removed.
       */
      void removeScope(const QString &scope);

    private:
      QString m_accName;
      QString m_accessToken;
      QString m_refreshToken;
      QStringList m_scopes;

      /**
       * Whether scopes were changed or not.
       *
       * KGoogle::Auth reads this attribute when Account is passed to
       * KGoogle::Auth::authenticate() to determine whether completely new
       * process of authentication is needed, or wheter just refreshing tokens
       * is enough.
       *
       * When m_scopesChanged is TRUE and KGoogle::Auth::authenticate() successffuly
       * performs full re-authentication it sets this attribute to FALSE and next time it
       * will just refresh existing tokens until the scopes are changed again.
       */
      bool m_scopesChanged;
  };

} /* namespace KGoogle */

Q_DECLARE_METATYPE(KGoogle::Account);
Q_DECLARE_METATYPE(KGoogle::Account*);

#endif // LIBKGOOGLE_ACCOUNT_H
