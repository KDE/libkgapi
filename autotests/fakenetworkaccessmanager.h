/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include <QByteArray>
#include <QList>
#include <QNetworkAccessManager>
#include <QUrl>

class FakeNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    class Scenario
    {
    public:
        Scenario()
        {
        }

        Scenario(const QUrl &requestUrl,
                 QNetworkAccessManager::Operation method,
                 const QByteArray &requestData,
                 int responseCode,
                 const QByteArray &responseData,
                 bool needsAuth = true)
            : requestUrl(requestUrl)
            , requestMethod(method)
            , requestData(requestData)
            , responseCode(responseCode)
            , responseData(responseData)
            , needsAuth(needsAuth)
        {
        }

        QUrl requestUrl;
        QNetworkAccessManager::Operation requestMethod;
        QList<QPair<QByteArray, QByteArray>> requestHeaders;
        QByteArray requestData;
        int responseCode = -1;
        QList<QPair<QByteArray, QByteArray>> responseHeaders;
        QByteArray responseData;
        bool needsAuth = true;
    };

    explicit FakeNetworkAccessManager(QObject *parent = nullptr);

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData) override;

private:
    QList<Scenario> mScenarios;
};
