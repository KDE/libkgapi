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

#ifndef LIBKGAPI_ACCOUNT_H
#define LIBKGAPI_ACCOUNT_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

#include <libkgapi/libkgapi_export.h>

namespace KGAPI
{

/**
 * Account class represents a single Google account.
 *
 * Note that any change in the Account (new access token, new refresh token,
 * changed scopes...), must be stored in the KWallet via
 * KGAPI::Auth::storeAccount().
 *
 * Any change to scopes (adding or removing) requires passing the Account object
 * to KGAPI::Auth::authenticate() to retrieve new tokens that would grant access
 * to the new set of scopes only
 */
class LIBKGAPI_EXPORT_DEPRECATED Account
{
  public:

    typedef QSharedPointer< Account > Ptr;

    /**
     * Creates any empty account.
     *
     * Such object represents invalid account.
     */
    Account();

    /**
     * Constructs a new account
     *
     * @param account Google account name (usually user.name@gmail.com)
     * @param accessToken Access token to \p scopes for \p account
     * @param refreshToken Refres token
     * @param scopes List of scopes
     */
    explicit Account(const QString &account, const QString &accessToken = QString(),
                     const QString &refreshToken = QString(),
                     const QList< QUrl > &scopes = QList< QUrl >());

    Account(const Account &other);

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
    QString accessToken() const;

    /**
     * Sets a new access token for account.
     *
     * @param accessToken The new access token.
     */
    void setAccessToken(const QString &accessToken);

    /**
     * @return Returns refresh token.
     */
    QString refreshToken() const;

    /**
     * Sets a new refresh token for the access token.
     *
     * @param refreshToken The new refresh token.
     */
    void setRefreshToken(const QString &refreshToken);

    /**
     * @return Returns list of scopes the account is authenticated against.
     */
    QList< QUrl > scopes() const;

    /**
     * Sets new scopes. Use must re-authenticate to allow access to the new scopes.
     *
     * @note Note that user must re-authenticate via KGAPI::Auth::authenticate() to
     * receive a new token that will grant access to the new list of scopes.
     *
     * @param scopes List of the new scopes.
     */
    void setScopes(const QList< QUrl > &scopes);

    /**
     * Adds a single scope to account scopes.
     *
     * @note Note that user must re-authenticate in KGAPI::Auth::authenticate() to
     * receive a new token that will grant access to the new list of scopes.
     *
     * @param scope The new scope to be added.
     */
    void addScope(const QUrl &scope);

    /**
     * Removes scope from the list.
     *
     * @note Note that user must re-authenticate KGAPI::Auth::authenticate() to
     * receive a new token that will grant access only to the new subset of scopes.
     *
     * @param scope The scope to be removed.
     */
    void removeScope(const QUrl &scope);

  private:
    class Private;
    Private * const d;
    friend class Private;

    /**
     * Grants access of KGAPI::Auth to KGAPI::Account::m_scopesChanged
     */
    friend class AuthPrivate;
    friend class Auth;

    /**
     * Whether scopes were changed or not.
     *
     * KGAPI::Auth reads this attribute when Account is passed to
     * KGAPI::Auth::authenticate() to determine whether completely new
     * process of authentication is needed, or whether just refreshing tokens
     * is enough.
     *
     * When m_scopesChanged is TRUE and KGAPI::Auth::authenticate() successffuly
     * performs full re-authentication it sets this attribute to FALSE and next time it
     * will just refresh existing tokens until the scopes are changed again.
     */
    bool m_scopesChanged; //krazy:exclude=dpointer
};

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Account::Ptr)
Q_DECLARE_METATYPE(KGAPI::Account*)

#endif // LIBKGAPI_ACCOUNT_H
