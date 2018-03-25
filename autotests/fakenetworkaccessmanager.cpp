/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fakenetworkaccessmanager.h"
#include "fakenetworkreply.h"
#include "fakenetworkaccessmanagerfactory.h"

#include <QNetworkRequest>
#include <QBuffer>
#include <QTest>

#define COMPARE(actual, expected) \
    do { \
        if (!QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__)) \
            return new FakeNetworkReply(op, originalReq); \
    } while (false)

#define VERIFY2(cond, msg) \
    do { \
        if (cond) { \
            if (!QTest::qVerify(true, #cond, (msg), __FILE__, __LINE__)) \
                return new FakeNetworkReply(op, originalReq); \
        } else { \
            if (!QTest::qVerify(false, #cond, (msg), __FILE__, __LINE__)) \
                return new FakeNetworkReply(op, originalReq); \
        } \
    } while (false);

FakeNetworkAccessManager::FakeNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QNetworkReply *FakeNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &originalReq,
                                                       QIODevice* outgoingData)
{
    auto namFactory = dynamic_cast<FakeNetworkAccessManagerFactory*>(KGAPI2::NetworkAccessManagerFactory::instance());
    VERIFY2(namFactory, "NAMFactory is nto a FakeNetworkAccessManagerFactory!");
    VERIFY2(namFactory->hasScenario(), "No scenario for request!");

    const auto scenario = namFactory->nextScenario();
    if (scenario.needsAuth) {
        VERIFY2(originalReq.hasRawHeader("Authorization"), "Missing Auth token header!");
    }

    COMPARE(scenario.requestUrl, originalReq.url());
    COMPARE(scenario.requestMethod, op);

    if (outgoingData) {
        COMPARE(outgoingData->readAll(), scenario.requestData);
    }

    return new FakeNetworkReply(scenario);
}


