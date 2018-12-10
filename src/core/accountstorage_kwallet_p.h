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

#ifndef LIBKGAPI2_ACCOUNTSTORAGE_KWALLET_P_H_
#define LIBKGAPI2_ACCOUNTSTORAGE_KWALLET_P_H_

#include "accountstorage_p.h"

namespace KWallet {
class Wallet;
}

namespace KGAPI2 {

class KWalletStorageFactory : public AccountStorageFactory
{
public:
    AccountStorage *create() const override;
};


class KWalletStorage : public AccountStorage
{
public:
    explicit KWalletStorage();
    ~KWalletStorage() override;

    void open(const std::function<void (bool)> &callback) override;
    bool opened() const override;

    AccountPtr getAccount(const QString &apiKey, const QString &accountName) override;
    bool storeAccount(const QString &apiKey, const AccountPtr &account) override;
    void removeAccount(const QString &apiKey, const QString &accountName) override;

private:
    AccountPtr parseAccount(const QString &str) const;
    QString serializeAccount(const AccountPtr &account) const;

    bool mWalletOpening = false;
    KWallet::Wallet *mWallet = nullptr;
};

}

#endif
