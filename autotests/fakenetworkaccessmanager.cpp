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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fakenetworkaccessmanager.h"
#include "fakenetworkreply.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include <QNetworkRequest>
#include <QBuffer>
#include <QTest>
#include <QProcess>

#include <iostream>

FakeNetworkAccessManager::FakeNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QNetworkReply *FakeNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &originalReq,
                                                       QIODevice* outgoingData)
{
    auto namFactory = dynamic_cast<FakeNetworkAccessManagerFactory*>(KGAPI2::NetworkAccessManagerFactory::instance());
    VERIFY2_RET(namFactory, "NAMFactory is nto a FakeNetworkAccessManagerFactory!",
                new FakeNetworkReply(op, originalReq));
    VERIFY2_RET(namFactory->hasScenario(), "No scenario for request!",
                new FakeNetworkReply(op, originalReq));

    const auto scenario = namFactory->nextScenario();
    if (scenario.needsAuth) {
        VERIFY2_RET(originalReq.hasRawHeader("Authorization"), "Missing Auth token header!",
                    new FakeNetworkReply(op, originalReq));
    }

    COMPARE_RET(scenario.requestUrl, originalReq.url(),
                new FakeNetworkReply(op, originalReq));
    if (op != QNetworkAccessManager::CustomOperation) {
        COMPARE_RET(scenario.requestMethod, op,
                    new FakeNetworkReply(op, originalReq));
    } else {
        const auto verb = originalReq.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray();
        COMPARE_RET(verb, QByteArray("PUT"), new FakeNetworkReply(op, originalReq));
    }
    for (const auto requestHeader : qAsConst(scenario.requestHeaders)) {
        VERIFY2_RET(originalReq.hasRawHeader(requestHeader.first),
                    qPrintable(QStringLiteral("Missing header '%1'").arg(QString::fromUtf8(requestHeader.first))),
                    new FakeNetworkReply(op, originalReq));
        COMPARE_RET(originalReq.rawHeader(requestHeader.first), requestHeader.second,
                    new FakeNetworkReply(op, originalReq));
    }

    if (outgoingData) {
        const auto actualRequest = outgoingData->readAll();
        if (actualRequest.startsWith('<')) {
            const auto formattedInput = reformatXML(actualRequest);
            const auto formattedExpected = reformatXML(scenario.requestData);
            if (formattedInput != formattedExpected) {
                std::cerr << diffData(formattedInput, formattedExpected).constData() << std::endl;
                FAIL_RET("Request data don't match!", new FakeNetworkReply(op, originalReq));
            }
        } else if (actualRequest.startsWith('{')) {
            const auto formattedInput = reformatJSON(actualRequest);
            const auto formattedExpected = reformatJSON(scenario.requestData);
            if (formattedInput != formattedExpected) {
                std::cerr << diffData(formattedInput, formattedExpected).constData() << std::endl;
                FAIL_RET("Request data don't match!", new FakeNetworkReply(op, originalReq));
            }
        } else {
            COMPARE_RET(actualRequest, scenario.requestData, new FakeNetworkReply(op, originalReq));
        }
    }

    return new FakeNetworkReply(scenario);
}


