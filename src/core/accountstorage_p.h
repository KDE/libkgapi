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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI2_ACCOUNTSTORAGE_P_H_
#define LIBKGAPI2_ACCOUNTSTORAGE_P_H_

#include "types.h"
#include "kgapicore_export.h"

#include <functional>

namespace KGAPI2 {

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
    virtual ~AccountStorage() {};
    virtual void open(const std::function<void(bool)> &callback) = 0;
    virtual bool opened() const = 0;

    virtual AccountPtr getAccount(const QString &apiKey, const QString &accountName) = 0;
    virtual bool storeAccount(const QString &apiKey, const AccountPtr &account) = 0;
    virtual void removeAccount(const QString &apiKey, const QString &accountName) = 0;
};

}

#endif
