/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2018 - 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "debug.h"
#include "networkaccessmanagerfactory_p.h"

#include <QNetworkAccessManager>

using namespace KGAPI2;

std::unique_ptr<NetworkAccessManagerFactory> NetworkAccessManagerFactory::sInstance;

class QtNetworkAccessManagerFactory : public NetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *networkAccessManager(QObject *parent) const override
    {
        auto *nam = new QNetworkAccessManager(parent);
        nam->setStrictTransportSecurityEnabled(true);
        nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
        return nam;
    }
};

void NetworkAccessManagerFactory::setFactory(NetworkAccessManagerFactory *factory)
{
    sInstance.reset(factory);
}

NetworkAccessManagerFactory *NetworkAccessManagerFactory::instance()
{
    if (!sInstance) {
        setFactory(new QtNetworkAccessManagerFactory);
    }
    return sInstance.get();
}
