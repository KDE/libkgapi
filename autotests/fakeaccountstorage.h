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

#ifndef KGAPI_FAKEACCOUNTSTORAGE_H
#define KGAPI_FAKEACCOUNTSTORAGE_H

#include "accountstorage_p.h"

#include <QMap>

class FakeAccountStorage : public KGAPI2::AccountStorage
{
public:
    explicit FakeAccountStorage();

    void open(const std::function<void (bool)> &callback) override;
    bool opened() const override;

    KGAPI2::AccountPtr getAccount(const QString &apiKey, const QString &accountName) override;
    bool storeAccount(const QString &apiKey, const KGAPI2::AccountPtr &account) override;
    void removeAccount(const QString &apiKey, const QString &accountName) override;

    KGAPI2::AccountPtr generateAccount(const QString &apiKey, const QString &accountName, const QList<QUrl> &scopes);

public:
    QMap<QString, KGAPI2::AccountPtr> mStore;
};


class FakeAccountStorageFactory : public KGAPI2::AccountStorageFactory
{
public:
    FakeAccountStorageFactory();
    ~FakeAccountStorageFactory() override;

    KGAPI2::AccountStorage *create() const override;
    FakeAccountStorage *fakeStore() const;

private:
    FakeAccountStorage *mStore;
};

#endif
