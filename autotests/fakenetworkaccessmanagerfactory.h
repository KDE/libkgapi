/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "../src/core/networkaccessmanagerfactory_p.h"
#include "fakenetworkaccessmanager.h"

#include <QList>

class FakeNetworkAccessManagerFactory : public KGAPI2::NetworkAccessManagerFactory
{
public:
    explicit FakeNetworkAccessManagerFactory() = default;

    static FakeNetworkAccessManagerFactory *get(); // instance+dynamic_cast

    void setScenarios(const QList<FakeNetworkAccessManager::Scenario> &scenarios);
    bool hasScenario() const;
    FakeNetworkAccessManager::Scenario nextScenario();

    QNetworkAccessManager *networkAccessManager(QObject *parent = nullptr) const override;

private:
    QList<FakeNetworkAccessManager::Scenario> mScenarios;
};

