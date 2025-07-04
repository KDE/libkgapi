/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "kgapicore_export.h"
#include "types.h"

#include <functional>

namespace KGAPI2
{

class AccountStorage;
// Exported for tests, otherwise a private class
class KGAPICORE_EXPORT AccountStorageFactory
{
public:
    static AccountStorageFactory *instance();

    virtual ~AccountStorageFactory();
    virtual AccountStorage *create() const = 0;

protected:
    explicit AccountStorageFactory();

    static AccountStorageFactory *sFactory;

private:
    Q_DISABLE_COPY(AccountStorageFactory)
};

class AccountStorage
{
public:
    virtual ~AccountStorage(){};
    virtual void getAccount(const QString &apiKey, const QString &accountName, const std::function<void(AccountPtr)> &callback) = 0;
    virtual void storeAccount(const QString &apiKey, const AccountPtr &account, const std::function<void(bool)> &callback) = 0;
    virtual void removeAccount(const QString &apiKey, const QString &accountName) = 0;
};

}
