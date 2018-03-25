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

#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakenetworkaccessmanagerfactory.h"
#include "fakenetworkaccessmanager.h"
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
        FakeNetworkAccessManager::Scenario scenario(
            QUrl(QStringLiteral("https://example.test/request")), QNetworkAccessManager::GetOperation,
            {}, 200, "Hello World!", false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({ scenario });
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->get(request), scenario);

        QVERIFY(!namf->hasScenario());
    }

    void testPostRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(
            QUrl(QStringLiteral("https://example.test/upload")), QNetworkAccessManager::PostOperation,
            "Uploading new data", 200, "Data stored", false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({ scenario });
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->post(request, scenario.requestData), scenario);

        QVERIFY(!namf->hasScenario());
    }

    void testPutRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(
            QUrl(QStringLiteral("https://example.test/create")), QNetworkAccessManager::PutOperation,
            "New data to store", 201, "New data stored", false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({ scenario });
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->put(request, scenario.requestData), scenario);

        QVERIFY(!namf->hasScenario());
    }


    void testDeleteRequest()
    {
        FakeNetworkAccessManager::Scenario scenario(
            QUrl(QStringLiteral("https://example.test/delete")), QNetworkAccessManager::DeleteOperation,
            {}, 204, {}, false);
        auto namf = FakeNetworkAccessManagerFactory::get();
        namf->setScenarios({ scenario });
        QScopedPointer<QNetworkAccessManager> nam(namf->networkAccessManager());

        QNetworkRequest request(scenario.requestUrl);
        checkReply(nam->deleteResource(request), scenario);

        QVERIFY(!namf->hasScenario());
    }
};

QTEST_GUILESS_MAIN(FakeNAMTest)

#include "fakenamtest.moc"
