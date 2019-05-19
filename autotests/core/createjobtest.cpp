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

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "createjob.h"
#include "account.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)

using Scenarios = QList<FakeNetworkAccessManager::Scenario>;

using namespace KGAPI2;

class TestCreateJob : public CreateJob
{
    Q_OBJECT

public:
    TestCreateJob(const QUrl &url, const QByteArray &data, QObject *parent = nullptr)
        : CreateJob(AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken")), parent)
        , mUrl(url)
        , mData(data)
    {
    }

    void start() override
    {
        QNetworkRequest request(mUrl);
        enqueueRequest(request, mData);
    }

    QByteArray response()
    {
        return mResponse;
    }

    void handleReply(const QNetworkReply *, const QByteArray &rawData) override
    {
        mResponse = rawData;
        emitFinished();
    }

private:
    QUrl mUrl;
    QByteArray mData;
    QByteArray mResponse;
};


class CreateJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testCreate_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");

        QTest::newRow("success") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::PostOperation,
              "New data", 200, "Data created" }
        };

        QTest::newRow("creation failed") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::PostOperation,
              "New data", KGAPI2::Forbidden, {} }
        };

        QTest::newRow("redirect") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::PostOperation,
              "New data", KGAPI2::TemporarilyMoved, "https://example.test/moved/data" },
            { QUrl(QStringLiteral("https://example.test/moved/data")), QNetworkAccessManager::PostOperation,
              "New data", KGAPI2::OK, "Data created" }
        };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto job = new TestCreateJob(scenarios.first().requestUrl, scenarios.first().requestData);
        QVERIFY(execJob(job));
        QCOMPARE(static_cast<int>(job->error()), scenarios.last().responseCode == 200 ? KGAPI2::NoError : scenarios.last().responseCode);
        QCOMPARE(job->response(), scenarios.last().responseData);

        QVERIFY(!FakeNetworkAccessManagerFactory::get()->hasScenario());
    }
};

QTEST_GUILESS_MAIN(CreateJobTest)

#include "createjobtest.moc"
