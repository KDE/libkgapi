/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "accountstorage_kwallet_p.h"
#include "account.h"
#include "../debug.h"

#include <KWallet/KWallet>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

using namespace KGAPI2;

namespace {

static const QString FolderName = QStringLiteral("LibKGAPI");
static const QString AccountNameKey = QStringLiteral("name");
static const QString ScopesKey = QStringLiteral("scopes");
static const QString AccessTokenKey = QStringLiteral("accessToken");
static const QString RefreshTokenKey = QStringLiteral("refreshToken");
static const QString ExpiresKey = QStringLiteral("expires");

}


AccountStorage *KWalletStorageFactory::create() const
{
    return new KWalletStorage();
}



KWalletStorage::KWalletStorage()
{
}

KWalletStorage::~KWalletStorage()
{
    delete mWallet;
}

void KWalletStorage::open(const std::function<void (bool)> &callback)
{
    const auto openedCallback = [=](bool opened) {
        mWalletOpening = false;
        if (!opened) {
            qCWarning(KGAPIDebug, "KWallet: failed to open");
            mWallet->deleteLater();
            mWallet = nullptr;
            callback(false);
            return;
        }

        if (mWallet->currentFolder() == FolderName) {
            callback(true);
            return;
        }

        if (!mWallet->hasFolder(FolderName)) {
            if (!mWallet->createFolder(FolderName)) {
                qCWarning(KGAPIDebug, "KWallet: failed to create a new folder");
                callback(false);
                return;
            }
        }
        if (!mWallet->setFolder(FolderName)) {
            qCWarning(KGAPIDebug, "KWallet: failed to change folder");
            callback(false);
            return;
        }

        callback(true);
    };

    if (mWallet) {
        if (mWallet->isOpen(KWallet::Wallet::NetworkWallet())) {
            callback(true);
            return;
        }

        if (mWalletOpening) {
            QObject::connect(mWallet, &KWallet::Wallet::walletOpened, openedCallback);
            return;
        }

        delete mWallet;
    }

    mWalletOpening = true;
    mWallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), 0, KWallet::Wallet::Asynchronous);
    if (!mWallet) {
        qCWarning(KGAPIDebug, "KWallet: failed to open wallet (maybe it's disabled?");
        callback(false);
    } else {
        QObject::connect(mWallet, &KWallet::Wallet::walletOpened, [this]() { mWalletOpening = false; });
        QObject::connect(mWallet, &KWallet::Wallet::walletOpened, openedCallback);
    }
}

bool KWalletStorage::opened() const
{
    return mWallet && mWallet->isOpen(KWallet::Wallet::NetworkWallet());
}

AccountPtr KWalletStorage::getAccount(const QString &apiKey, const QString &accountName)
{
    if (!opened()) {
        qCWarning(KGAPIDebug, "Trying to get an account from a closed wallet!");
        return {};
    }

    QMap<QString, QString> accounts;
    mWallet->readMap(apiKey, accounts);
    const auto accountIt = accounts.constFind(accountName);
    if (accountIt == accounts.cend()) {
        return {};
    }

    return parseAccount(*accountIt);
}

bool KWalletStorage::storeAccount(const QString &apiKey, const AccountPtr &account)
{
    if (!opened()) {
        qCWarning(KGAPIDebug, "Trying to store an account in a closed wallet!");
        return false;
    }

    QMap<QString, QString> accounts;
    if (mWallet->readMap(apiKey, accounts) == 0) {
        accounts[account->accountName()] = serializeAccount(account);
        if (mWallet->writeMap(apiKey, accounts) != 0) {
            qCWarning(KGAPIDebug, "KWallet: failed to write accounts map");
            return false;
        }
    } else {
        qCWarning(KGAPIDebug, "KWallet: failed to read accounts map");
        return false;
    }

    return true;
}

void KWalletStorage::removeAccount(const QString &apiKey, const QString &accountName)
{
    if (!opened()) {
        qCWarning(KGAPIDebug, "Trying to remove an account from a closed wallet!");
        return;
    }

    QMap<QString, QString> accounts;
    if (mWallet->readMap(apiKey, accounts) == 0) {
        if (accounts.remove(accountName)) {
            if (!mWallet->writeMap(apiKey, accounts)) {
                qCWarning(KGAPIDebug, "KWallet: failed to write accounts map");
            }
        }
    } else {
        qCWarning(KGAPIDebug, "KWallet: failed to read accounts map");
    }
}

AccountPtr KWalletStorage::parseAccount(const QString &str) const
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
    std::transform(scopesArray.constBegin(), scopesArray.constEnd(), std::back_inserter(scopes),
                    [](const QJsonValue &val) { return QUrl::fromEncoded(val.toString().toUtf8()); });
    auto acc = AccountPtr::create(obj.value(AccountNameKey).toString(),
                                    obj.value(AccessTokenKey).toString(),
                                    obj.value(RefreshTokenKey).toString(),
                                    scopes);
    acc->setExpireDateTime(QDateTime::fromString(obj.value(ExpiresKey).toString(), Qt::ISODate));
    return acc;
}

QString KWalletStorage::serializeAccount(const AccountPtr &account) const
{
    QJsonArray scopesArray;
    const auto scopes = account->scopes();
    std::transform(scopes.cbegin(), scopes.cend(), std::back_inserter(scopesArray),
                    [](const QUrl &scope) { return scope.toString(QUrl::FullyEncoded); });
    return QString::fromUtf8(
        QJsonDocument({ { AccountNameKey, account->accountName() },
                        { AccessTokenKey, account->accessToken() },
                        { RefreshTokenKey, account->refreshToken() },
                        { ExpiresKey, account->expireDateTime().toString(Qt::ISODate) },
                        { ScopesKey, scopesArray } }).toJson(QJsonDocument::Compact));
}
