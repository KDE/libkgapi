/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "networkaccessmanagerfactory_p.h"
#include "../debug.h"

#include <KIO/AccessManager>

using namespace KGAPI2;

NetworkAccessManagerFactory *NetworkAccessManagerFactory::sInstance = nullptr;

class KIONetworkAccessManagerFactory : public NetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *networkAccessManager(QObject *parent) const override
    {
        return new KIO::Integration::AccessManager(parent);
    }
};


NetworkAccessManagerFactory::NetworkAccessManagerFactory()
{
}

NetworkAccessManagerFactory::~NetworkAccessManagerFactory()
{
}

void NetworkAccessManagerFactory::setFactory(NetworkAccessManagerFactory *factory)
{
    delete sInstance;
    sInstance = factory;
}

NetworkAccessManagerFactory *NetworkAccessManagerFactory::instance()
{
    if (!sInstance) {
        sInstance = new KIONetworkAccessManagerFactory();
    }
    return sInstance;
}
