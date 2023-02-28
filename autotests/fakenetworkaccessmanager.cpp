/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fakenetworkaccessmanager.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "fakenetworkreply.h"
#include "testutils.h"

#include <QNetworkRequest>

#include <iostream>

FakeNetworkAccessManager::FakeNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QNetworkReply *FakeNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData)
{
    auto namFactory = dynamic_cast<FakeNetworkAccessManagerFactory *>(KGAPI2::NetworkAccessManagerFactory::instance());
    VERIFY2_RET(namFactory, "NAMFactory is nto a FakeNetworkAccessManagerFactory!", new FakeNetworkReply(op, originalReq));
    VERIFY2_RET(namFactory->hasScenario(), "No scenario for request!", new FakeNetworkReply(op, originalReq));

    const auto scenario = namFactory->nextScenario();
    if (scenario.needsAuth) {
        VERIFY2_RET(originalReq.hasRawHeader("Authorization"), "Missing Auth token header!", new FakeNetworkReply(op, originalReq));
    }

    COMPARE_RET(scenario.requestUrl, originalReq.url(), new FakeNetworkReply(op, originalReq));
    if (op != QNetworkAccessManager::CustomOperation) {
        COMPARE_RET(scenario.requestMethod, op, new FakeNetworkReply(op, originalReq));
    } else {
        const auto verb = originalReq.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray();
        // In the People API some of the requests ask for a custom verb called "PATCH", so allow this.
        // Additionally we use a modify job for photo deletion requests, which return the modified person.
        // This uses the "DELETE" verb but, since it acts as a modification on the person itself, we use
        // a modify job and we should therefore accept this verb here.
        if (verb != QByteArray("PATCH") && verb != QByteArray("PUT") && verb != QByteArray("DELETE")) {
            FAIL_RET("Invalid verb", new FakeNetworkReply(op, originalReq));
        }
    }
    for (const auto &requestHeader : std::as_const(scenario.requestHeaders)) {
        VERIFY2_RET(originalReq.hasRawHeader(requestHeader.first),
                    qPrintable(QStringLiteral("Missing header '%1'").arg(QString::fromUtf8(requestHeader.first))),
                    new FakeNetworkReply(op, originalReq));
        COMPARE_RET(originalReq.rawHeader(requestHeader.first), requestHeader.second, new FakeNetworkReply(op, originalReq));
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
