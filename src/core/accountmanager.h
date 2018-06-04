/*
    Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_ACCOUNTMANAGER_H_
#define LIBKGAPI2_ACCOUNTMANAGER_H_

#include <QObject>
#include <QList>

#include "account.h"
#include "kgapicore_export.h"


class QUrl;

namespace KGAPI2 {

class AccountManager;
/**
 * @brief AccountPromise is a result of asynchronous operations of AccountManager
 *
 * The AccountPromise is returned by AccountManager and will emit @p finished()
 * signal when the asynchronous retrieval of the Account has finished.
 *
 * The object will delete itself once @p finished signal is emitted.
 */
class KGAPICORE_EXPORT AccountPromise : public QObject
{
    Q_OBJECT
public:
    ~AccountPromise() override;

    AccountPtr account() const;

    bool hasError() const;
    QString errorText() const;

Q_SIGNALS:
    /**
     * @brief The retrieval has finished and the Account can be retrieved.
     *
     * The object is automatically scheduled for deletion after this signal
     * is emitted.
     */
    void finished(AccountPromise *self);

private:
    AccountPromise(QObject *parent = nullptr);
    Q_DISABLE_COPY(AccountPromise)

    friend class AccountManager;
    class Private;
    QScopedPointer<Private> const d;
};

class KGAPICORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
public:
    ~AccountManager() override;

    static AccountManager *instance();

    /**
     * @brief Asynchronously returns an authenticated account for given conditions
     *
     * The returned account is guaranteed to be authenticated against at least the
     * requested @p scopes, but it may be authenticated against more scopes.
     * If an account for the given @p apiKey and @p accountName already exists
     * but is not authenticated against all the scopes the user will be presented
     * with a prompt to confirm the missing scopes.
     *
     * If no such account exists, user will be prompted with full authentication
     * process.
     *
     * The returned account is guaranteed to be authenticated, howerver the tokens
     * may be expired. It's up to the caller to ensure the tokens are refreshed
     * using @p refreshTokens method.
     *
     * @p apiSecret is only used to authetnicate new account or missing scopes
     * and is not stored anywhere.
     *
     * @see refreshTokens, hasAccount
     */
    AccountPromise *getAccount(const QString &apiKey, const QString &apiSecret,
                               const QString &accountName,
                               const QList<QUrl> &scopes);


    /**
     * @brief Asynchronously refreshes tokens in given Account
     *
     * The returned account is guaranteed to be authenticated against at least
     * the requested @p scopes, but it may be authenticated against more scopes.
     * If an account does not exist, it will be created (see AccountManager::getAccount()).
     *
     * @p apiSecret is onyl used to authenticate a new account.
     *
     * @see getAccount
     */
    AccountPromise *refreshTokens(const QString &apiKey, const QString &apiSecret,
                                  const QString &accountName);

    /**
     * @brief Asynchronously checks whether the specified account exists.
     *
     * Optionally, when non-empty list of @p scopes is provided this method also
     * checks whether, if the account exists, it is authenticated against the
     * provided scopes.
     *
     * The AccountPromise will have the discovered account set if it is found,
     * otherwise it's set to null.
     */
    AccountPromise *findAccount(const QString &aipKey, const QString &accountName,
                                const QList<QUrl> &scopes = {});


    /**
     * @brief Asynchronously remove given scopes from authenticated account.
     *
     * Removes the given scopes from the account authenticated scopes, so that
     * next time the account is requested with any of the removed scopes the user
     * will be presented with a prompt to confirm access again.
     */
    void removeScopes(const QString &apiKey, const QString &accountName,
                      const QList<QUrl> &removeScopes);

protected:
    explicit AccountManager(QObject *parent = nullptr);
    Q_DISABLE_COPY(AccountManager)

    static AccountManager *sInstance;
    class Private;
    QScopedPointer<Private> const d;
};

}

#endif
