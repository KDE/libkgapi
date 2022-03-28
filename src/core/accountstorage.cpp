/*
    SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "accountstorage_kwallet_p.h"
#include "accountstorage_p.h"

using namespace KGAPI2;

AccountStorageFactory *AccountStorageFactory::sFactory = nullptr;

AccountStorageFactory *AccountStorageFactory::instance()
{
    if (!sFactory) {
        sFactory = new KWalletStorageFactory();
    }
    return sFactory;
}

AccountStorageFactory::AccountStorageFactory()
{
}

AccountStorageFactory::~AccountStorageFactory()
{
}
