/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fakeaccountstorage.h"
#include "account.h"

#include <QUuid>
#include <QDateTime>

using namespace KGAPI2;

FakeAccountStorage::FakeAccountStorage()
{
}

void FakeAccountStorage::open(const std::function<void (bool)> &callback)
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

