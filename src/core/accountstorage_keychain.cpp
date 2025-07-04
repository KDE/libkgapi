/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "account.h"
#include "accountstorage_keychain_p.h"
#include "debug.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <qt6keychain/keychain.h>

using namespace KGAPI2;
using namespace Qt::StringLiterals;

namespace
{

static constexpr QLatin1StringView FolderName = "LibKGAPI"_L1;
static constexpr QLatin1StringView AccountNameKey = "name"_L1;
static constexpr QLatin1StringView ScopesKey = "scopes"_L1;
static constexpr QLatin1StringView AccessTokenKey = "accessToken"_L1;
static constexpr QLatin1StringView RefreshTokenKey = "refreshToken"_L1;
static constexpr QLatin1StringView ExpiresKey = "expires"_L1;

}

AccountStorage *KeyChainStorageFactory::create() const
{
    return new KeyChainStorage();
}

KeyChainStorage::KeyChainStorage() = default;
KeyChainStorage::~KeyChainStorage() = default;

void KeyChainStorage::getAccount(const QString &apiKey, const QString &accountName, const std::function<void(AccountPtr)> &callback)
{
    auto job = new QKeychain::ReadPasswordJob(FolderName);
    job->setKey(apiKey);

    QObject::connect(job, &QKeychain::Job::finished, job, [this, job, callback, accountName]() {
        if (job->error() != QKeychain::Error::NoError) {
            qCWarning(KGAPIDebug) << "Unable to read password:" << job->errorString();
            callback({});
            return;
        }

        QMap<QString, QString> accounts;
        auto value = job->binaryData();

        QDataStream ds(value);
        ds >> accounts;

        const auto accountIt = accounts.constFind(accountName);
        if (accountIt == accounts.cend()) {
            qCWarning(KGAPIDebug) << "Unable to find:" << accountName;
            callback({});
            return;
        }

        callback(parseAccount(*accountIt));
    });
    job->start();
}

void KeyChainStorage::storeAccount(const QString &apiKey, const AccountPtr &account, const std::function<void(bool)> &callback)
{
    auto job = new QKeychain::ReadPasswordJob(FolderName);
    job->setKey(apiKey);

    QObject::connect(job, &QKeychain::Job::finished, job, [this, apiKey, job, callback, account]() {
        QMap<QString, QString> accounts;

        if (job->error() == QKeychain::Error::NoError) {
            // we found some existing entries, keep them
            auto value = job->binaryData();
            QDataStream ds(value);
            ds >> accounts;
        }

        accounts[account->accountName()] = serializeAccount(account);

        QByteArray mapData;
        QDataStream ds(&mapData, QIODeviceBase::WriteOnly);
        ds << accounts;

        auto writeJob = new QKeychain::WritePasswordJob(FolderName);
        writeJob->setKey(apiKey);
        writeJob->setBinaryData(mapData);

        QObject::connect(writeJob, &QKeychain::Job::finished, writeJob, [writeJob, callback]() {
            QMap<QString, QString> accounts;
            if (writeJob->error() != QKeychain::Error::NoError) {
                qCWarning(KGAPIDebug) << "Unable to write new account:" << writeJob->errorString();
                callback(false);
                return;
            }

            callback(true);
        });
        writeJob->start();
    });
    job->start();
}

void KeyChainStorage::removeAccount(const QString &apiKey, const QString &accountName)
{
    auto job = new QKeychain::ReadPasswordJob(FolderName);
    job->setKey(apiKey);

    QObject::connect(job, &QKeychain::Job::finished, job, [apiKey, job, accountName]() {
        if (job->error() != QKeychain::Error::NoError) {
            qCWarning(KGAPIDebug) << "Could not find existing entries:" << job->errorString();
            return;
        }

        QMap<QString, QString> accounts;
        auto value = job->binaryData();
        QDataStream ds(value);
        ds >> accounts;

        if (!accounts.remove(accountName)) {
            // entry doesn't exist
            return;
        }

        QByteArray mapData;
        QDataStream dsOut(&mapData, QIODeviceBase::WriteOnly);
        dsOut << accounts;

        auto writeJob = new QKeychain::WritePasswordJob(FolderName);
        writeJob->setKey(apiKey);
        writeJob->setBinaryData(mapData);

        QObject::connect(writeJob, &QKeychain::Job::finished, writeJob, [writeJob]() {
            if (writeJob->error() != QKeychain::Error::NoError) {
                qCWarning(KGAPIDebug, "Keychain: failed to write accounts map");
                return;
            }
        });
        writeJob->start();
    });
}

AccountPtr KeyChainStorage::parseAccount(const QString &str) const
{
    const auto doc = QJsonDocument::fromJson(str.toUtf8());
    if (doc.isNull()) {
        qCWarning(KGAPIDebug, "Failed to parse account returned from KWallet");
        return {};
    }
    const auto obj = doc.object();
    QJsonArray scopesArray = obj.value(ScopesKey).toArray();
    QList<QUrl> scopes;
    scopes.reserve(scopesArray.size());
    std::transform(scopesArray.constBegin(), scopesArray.constEnd(), std::back_inserter(scopes), [](const QJsonValue &val) {
        return QUrl::fromEncoded(val.toString().toUtf8());
    });
    auto acc = AccountPtr::create(obj.value(AccountNameKey).toString(), obj.value(AccessTokenKey).toString(), obj.value(RefreshTokenKey).toString(), scopes);
    acc->setExpireDateTime(QDateTime::fromString(obj.value(ExpiresKey).toString(), Qt::ISODate));
    return acc;
}

QString KeyChainStorage::serializeAccount(const AccountPtr &account) const
{
    QJsonArray scopesArray;
    const auto scopes = account->scopes();
    std::transform(scopes.cbegin(), scopes.cend(), std::back_inserter(scopesArray), [](const QUrl &scope) {
        return scope.toString(QUrl::FullyEncoded);
    });
    return QString::fromUtf8(QJsonDocument({{AccountNameKey, account->accountName()},
                                            {AccessTokenKey, account->accessToken()},
                                            {RefreshTokenKey, account->refreshToken()},
                                            {ExpiresKey, account->expireDateTime().toString(Qt::ISODate)},
                                            {ScopesKey, scopesArray}})
                                 .toJson(QJsonDocument::Compact));
}
