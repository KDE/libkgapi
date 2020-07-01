/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
