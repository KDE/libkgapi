/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

class QNetworkAccessManager;
class QObject;

#include "kgapicore_export.h"

#include <memory>

namespace KGAPI2 {

// Export for use in unit-tests, header not installed though
class KGAPICORE_EXPORT NetworkAccessManagerFactory
{
public:
    virtual ~NetworkAccessManagerFactory() = default;

    static NetworkAccessManagerFactory *instance();
    static void setFactory(NetworkAccessManagerFactory *factory);

    virtual QNetworkAccessManager *networkAccessManager(QObject *parent = nullptr) const = 0;

protected:
    static std::unique_ptr<NetworkAccessManagerFactory> sInstance;

    explicit NetworkAccessManagerFactory() = default;
};

}

