/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakeaccountstorage.h"
#include "account.h"

#include <QDateTime>
#include <QUuid>

using namespace KGAPI2;

FakeAccountStorage::FakeAccountStorage()
{
}

void FakeAccountStorage::open(const std::function<void(bool)> &callback)
{
    callback(true);
}

bool FakeAccountStorage::opened() const
{
    return true;
}

AccountPtr FakeAccountStorage::getAccount(const QString &apiKey, const QString &accountName)
{
    return mStore.value(apiKey + accountName, {});
}

bool FakeAccountStorage::storeAccount(const QString &apiKey, const KGAPI2::AccountPtr &account)
{
    mStore[apiKey + account->accountName()] = account;
    return true;
}

void FakeAccountStorage::removeAccount(const QString &apiKey, const QString &accountName)
{
    mStore.remove(apiKey + accountName);
}

AccountPtr FakeAccountStorage::generateAccount(const QString &apiKey, const QString &accountName, const QList<QUrl> &scopes)
{
    const auto acc = AccountPtr::create(accountName, QUuid::createUuid().toString(), QUuid::createUuid().toString(), scopes);
    acc->setExpireDateTime(QDateTime::currentDateTime().addDays(1));
    storeAccount(apiKey, acc);
    return acc;
}

FakeAccountStorageFactory::FakeAccountStorageFactory()
{
    mStore = new FakeAccountStorage();
    sFactory = this;
}

FakeAccountStorageFactory::~FakeAccountStorageFactory()
{
    delete mStore;
    sFactory = nullptr;
}

KGAPI2::AccountStorage *FakeAccountStorageFactory::create() const
{
    return mStore;
}

FakeAccountStorage *FakeAccountStorageFactory::fakeStore() const
{
    return mStore;
}
