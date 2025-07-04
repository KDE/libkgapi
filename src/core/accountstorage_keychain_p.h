/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "accountstorage_p.h"

namespace KGAPI2
{

class KeyChainStorageFactory : public AccountStorageFactory
{
public:
    AccountStorage *create() const override;
};

class KeyChainStorage : public AccountStorage
{
public:
    explicit KeyChainStorage();
    ~KeyChainStorage() override;

    void getAccount(const QString &apiKey, const QString &accountName, const std::function<void(AccountPtr)> &callback) override;
    void storeAccount(const QString &apiKey, const AccountPtr &account, const std::function<void(bool)> &callback) override;
    void removeAccount(const QString &apiKey, const QString &accountName) override;

private:
    AccountPtr parseAccount(const QString &str) const;
    QString serializeAccount(const AccountPtr &account) const;
};

}
