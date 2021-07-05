/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "accountmanager.h"
#include "authjob.h"
#include "accountstorage_p.h"
#include "../debug.h"

#include <QTimer>
#include <QDateTime>

#include <functional>

namespace KGAPI2 {

AccountManager *AccountManager::sInstance = nullptr;

class AccountPromise::Private
{
public:
    Private(AccountPromise *q)
        : q(q)

    {}

    void setError(const QString &error)
    {
        this->error = error;
        emitFinished();
    }

    void setAccount(const AccountPtr &account)
    {
        this->account = account;
        emitFinished();
    }

    void setRunning()
    {
        mRunning = true;
    }

    bool isRunning() const
    {
        return mRunning;
    }


    QString error;
    AccountPtr account;
private:
    void emitFinished()
    {
        QTimer::singleShot(0, q, [this]() {
            Q_EMIT q->finished(q);
            q->deleteLater();
        });
    }

    bool mRunning = false;
    AccountPromise * const q;
};

class AccountManager::Private
{
public:
    Private(AccountManager *q)
        : q(q)
    {}

    void updateAccount(AccountPromise *promise, const QString &apiKey, const QString &apiSecret,
                       const AccountPtr &account, const QList<QUrl> &requestedScopes)
    {
        if (!requestedScopes.isEmpty()) {
            auto currentScopes = account->scopes();
            for (const auto &requestedScope : requestedScopes) {
                if (!currentScopes.contains(requestedScope)) {
                    currentScopes.push_back(requestedScope);
                }
            }
            if (currentScopes != account->scopes()) {
                account->setScopes(currentScopes);
            }
        }
        auto *job = new AuthJob(account, apiKey, apiSecret);
        job->setUsername(account->accountName());
        connect(job, &AuthJob::finished,
                [=]() {
                    if (job->error() != KGAPI2::NoError) {
                        promise->d->setError(tr("Failed to authenticate additional scopes"));
                        return;
                    }

                    mStore->storeAccount(apiKey, job->account());
                    promise->d->setAccount(job->account());
                });
    }

    void createAccount(AccountPromise *promise, const QString &apiKey, const QString &apiSecret,
                       const QString &accountName, const QList<QUrl> &scopes)
    {
        const auto account = AccountPtr::create(accountName, QString{}, QString{}, scopes);
        updateAccount(promise, apiKey, apiSecret, account, {});
    }

    bool compareScopes(const QList<QUrl> &currentScopes, const QList<QUrl> &requestedScopes) const {
        for (const auto &scope : std::as_const(requestedScopes)) {
            if (!currentScopes.contains(scope)) {
                return false;
            }
        }
        return true;
    }

    void ensureStore(const std::function<void(bool)> &callback)
    {
        if (!mStore) {
            mStore = AccountStorageFactory::instance()->create();
        }
        if (!mStore->opened()) {
            mStore->open(callback);
        } else {
            callback(true);
        }
    }

    AccountPromise *createPromise(const QString &apiKey, const QString &accountName)
    {
        const QString key = apiKey + accountName;
        auto promise = mPendingPromises.value(key, nullptr);
        if (!promise) {
            promise = new AccountPromise(q);
            QObject::connect(promise, &QObject::destroyed,
                             q, [key, this]() {
                                 mPendingPromises.remove(key);
                             });
            mPendingPromises.insert(key, promise);
        }
        return promise;
    }
public:
    AccountStorage *mStore = nullptr;

private:
    QHash<QString, AccountPromise*> mPendingPromises;

    AccountManager * const q;
};

}

using namespace KGAPI2;


AccountPromise::AccountPromise(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

AccountPromise::~AccountPromise()
{
}

AccountPtr AccountPromise::account() const
{
    return d->account;
}

bool AccountPromise::hasError() const
{
    return !d->error.isNull();
}

QString AccountPromise::errorText() const
{
    return d->error;
}


AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

AccountManager::~AccountManager()
{
}

AccountManager *AccountManager::instance()
{
    if (!sInstance) {
        sInstance = new AccountManager;
    }
    return sInstance;
}

AccountPromise *AccountManager::getAccount(const QString &apiKey, const QString &apiSecret,
                                           const QString &accountName,
                                           const QList<QUrl> &scopes)
{
    auto promise = d->createPromise(apiKey, accountName);
    if (!promise->d->isRunning()) {
        // Start the process asynchronously so that caller has a chance to connect
        // to AccountPromise signals.
        QTimer::singleShot(0, this, [=]() {
            d->ensureStore([=](bool storeOpened) {
                if (!storeOpened) {
                    promise->d->setError(tr("Failed to open account store"));
                    return;
                }

                const auto account = d->mStore->getAccount(apiKey, accountName);
                if (!account) {
                    d->createAccount(promise, apiKey, apiSecret, accountName, scopes);
                } else {
                    if (d->compareScopes(account->scopes(), scopes)) {
                        // Don't hand out obviously expired tokens
                        if (account->expireDateTime() <= QDateTime::currentDateTime()) {
                            d->updateAccount(promise, apiKey, apiSecret, account, scopes);
                        } else {
                            promise->d->setAccount(account);
                        }
                    } else {
                        // Since installed apps can't keep the API secret truly a secret
                        // incremental authorization is not allowed by Google so we need
                        // to request a completely new token from scratch.
                        account->setAccessToken({});
                        account->setRefreshToken({});
                        account->setExpireDateTime({});
                        d->updateAccount(promise, apiKey, apiSecret, account, scopes);
                    }
                }
            });
        });
        promise->d->setRunning();
    }
    return promise;
}

AccountPromise *AccountManager::refreshTokens(const QString &apiKey, const QString &apiSecret,
                                              const QString &accountName)
{
    auto promise = d->createPromise(apiKey, accountName);
    if (!promise->d->isRunning()) {
        QTimer::singleShot(0, this, [=]() {
            d->ensureStore([=](bool storeOpened) {
                if (!storeOpened) {
                    promise->d->setError(tr("Failed to open account store"));
                    return;
                }

                const auto account = d->mStore->getAccount(apiKey, accountName);
                if (!account) {
                    promise->d->setAccount({});
                } else {
                    d->updateAccount(promise, apiKey, apiSecret, account, {});
                }
            });
        });
        promise->d->setRunning();
    }
    return promise;
}


AccountPromise *AccountManager::findAccount(const QString &apiKey, const QString &accountName,
                                            const QList<QUrl> &scopes)
{
    auto promise = d->createPromise(apiKey, accountName);
    if (!promise->d->isRunning()) {
        QTimer::singleShot(0, this, [=]() {
            d->ensureStore([=](bool storeOpened) {
                if (!storeOpened) {
                    promise->d->setError(tr("Failed to open account store"));
                    return;
                }

                const auto account = d->mStore->getAccount(apiKey, accountName);
                if (!account) {
                    promise->d->setAccount({});
                } else {
                    const auto currentScopes = account->scopes();
                    if (scopes.isEmpty() || d->compareScopes(currentScopes, scopes)) {
                        promise->d->setAccount(account);
                    } else {
                        promise->d->setAccount({});
                    }
                }
            });
        });
        promise->d->setRunning();
    }
    return promise;
}

void AccountManager::removeScopes(const QString &apiKey, const QString &accountName,
                                  const QList<QUrl> &removedScopes)
{
    d->ensureStore([=](bool storeOpened) {
        if (!storeOpened) {
            return;
        }

        const auto account = d->mStore->getAccount(apiKey, accountName);
        if (!account) {
            return;
        }

        for (const auto &scope : removedScopes) {
            account->removeScope(scope);
        }
        if (account->scopes().isEmpty()) {
            d->mStore->removeAccount(apiKey, account->accountName());
        } else {
            // Since installed apps can't keep the API secret truly a secret
            // incremental authorization is not allowed by Google so we need
            // to request a completely new token from scratch.
            account->setAccessToken({});
            account->setRefreshToken({});
            account->setExpireDateTime({});
            d->mStore->storeAccount(apiKey, account);
        }
    });
}
