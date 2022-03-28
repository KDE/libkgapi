/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QSignalSpy>
#include <QTest>

#include "fakenetworkaccessmanager.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "fakenetworkreply.h"

class FakeNAMTest : public QObject
{
    Q_OBJECT

private:
    void checkReply(QNetworkReply *reply, const FakeNetworkAccessManager::Scenario &scenario)
    {
        QSignalSpy readSpy(reply, &QNetworkReply::readyRead);
        QSignalSpy finishedSpy(reply, &QNetworkReply::finished);

        QVERIFY(finishedSpy.wait());
        QCOMPARE(readSpy.count(), 1);
        QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), scenario.responseCode);
        QCOMPARE(reply->header(QNetworkRequest::ContentLengthHeader).toInt(), scenario.responseData.size());
        QCOMPARE(reply->header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("text/plain"));
        QCOMPARE(reply->readAll(), scenario.responseData);
        delete reply;
    }

private Q_SLOTS:
    void initTestCase()
    {
        KGAPI2::NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
        QVERIFY(FakeNetworkAccessManagerFactory::get());
    }

    void testGetRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(QUrl(QStringLiteral("https://example.test/request")),
                                                    QNetworkAccessManager::GetOperation,
                                                    {},
                                                    200,
                                                    "Hello World!",
                                                    false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({scenario});
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->get(request), scenario);

        QVERIFY(!namf->hasScenario());
    }

    void testPostRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(QUrl(QStringLiteral("https://example.test/upload")),
                                                    QNetworkAccessManager::PostOperation,
                                                    "Uploading new data",
                                                    200,
                                                    "Data stored",
                                                    false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({scenario});
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->post(request, scenario.requestData), scenario);

        QVERIFY(!namf->hasScenario());
    }

    void testPutRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(QUrl(QStringLiteral("https://example.test/create")),
                                                    QNetworkAccessManager::PutOperation,
                                                    "New data to store",
                                                    201,
                                                    "New data stored",
                                                    false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({scenario});
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->put(request, scenario.requestData), scenario);

        QVERIFY(!namf->hasScenario());
    }

    void testDeleteRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(QUrl(QStringLiteral("https://example.test/delete")),
                                                    QNetworkAccessManager::DeleteOperation,
                                                    {},
                                                    204,
                                                    {},
                                                    false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({scenario});
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->deleteResource(request), scenario);

        QVERIFY(!namf->hasScenario());
    }
};

QTEST_GUILESS_MAIN(FakeNAMTest)

#include "fakenamtest.moc"
