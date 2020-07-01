/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakenetworkaccessmanagerfactory.h"

FakeNetworkAccessManagerFactory::FakeNetworkAccessManagerFactory()
{
}

void FakeNetworkAccessManagerFactory::setScenarios(const QList<FakeNetworkAccessManager::Scenario> &scenarios)
{
    mScenarios = scenarios;
}

FakeNetworkAccessManagerFactory * FakeNetworkAccessManagerFactory::get()
{
    return dynamic_cast<FakeNetworkAccessManagerFactory*>(instance());
}

bool FakeNetworkAccessManagerFactory::hasScenario() const
{
    return !mScenarios.isEmpty();
}

FakeNetworkAccessManager::Scenario FakeNetworkAccessManagerFactory::nextScenario()
{
    return mScenarios.takeFirst();
}

QNetworkAccessManager *FakeNetworkAccessManagerFactory::networkAccessManager(QObject *parent) const
{
    return new FakeNetworkAccessManager(parent);
}
