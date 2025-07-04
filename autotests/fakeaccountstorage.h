/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "accountstorage_p.h"

#include <QMap>

class FakeAccountStorage : public KGAPI2::AccountStorage
{
public:
    explicit FakeAccountStorage();

    void getAccount(const QString &apiKey, const QString &accountName, const std::function<void(KGAPI2::AccountPtr)> &callback) override;
    void storeAccount(const QString &apiKey, const KGAPI2::AccountPtr &account, const std::function<void(bool)> &callback) override;
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
