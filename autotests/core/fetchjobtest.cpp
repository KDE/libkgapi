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

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "fetchjob.h"
#include "account.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)

using Scenarios = QList<FakeNetworkAccessManager::Scenario>;

using namespace KGAPI2;

class TestFetchJob : public FetchJob
{
    Q_OBJECT

public:
    explicit TestFetchJob(const QUrl &url, QObject *parent = nullptr)
        : FetchJob(parent)
        , mUrl(url)
    {
    }

    TestFetchJob(const AccountPtr &account, const QUrl &url, QObject *parent = nullptr)
        : FetchJob(account, parent)
        , mUrl(url)
    {
    }

    void start() override
    {
        enqueueRequest(QNetworkRequest(mUrl));
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
    QByteArray mResponse;
};


class FetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testUnauthenticatedFetch_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");

        QTest::newRow("success") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::GetOperation,
              {}, 200, "Test Response" }
        };

        QTest::newRow("not found") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/does/not/exist")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::NotFound, {} }
        };

        QTest::newRow("redirect") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::TemporarilyMoved, "https://example.test/moved/data" },
            { QUrl(QStringLiteral("https://example.test/moved/data")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::OK, "Here's your data" }
        };
    }

    void testUnauthenticatedFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto job = new TestFetchJob(scenarios.first().requestUrl);
        QVERIFY(execJob(job));
        QCOMPARE(job->error(), scenarios.last().responseCode == 200 ? KGAPI2::NoError : scenarios.last().responseCode);
        QCOMPARE(job->response(), scenarios.last().responseData);

        QVERIFY(!FakeNetworkAccessManagerFactory::get()->hasScenario());
    }

    void testAuthenticatedFetch_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");

        QTest::newRow("success") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::GetOperation,
              {}, 200, "Response" }
        };

        QTest::newRow("token expired") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::Unauthorized, {} }
        };
    }

    void testAuthenticatedFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create();
        auto job = new TestFetchJob(account, scenarios.first().requestUrl);
        QVERIFY(execJob(job));
        QCOMPARE(job->error(), scenarios.last().responseCode == 200 ? KGAPI2::NoError : scenarios.last().responseCode);
        QCOMPARE(job->response(), scenarios.last().responseData);

        QVERIFY(!FakeNetworkAccessManagerFactory::get()->hasScenario());
    }
};

QTEST_MAIN(FetchJobTest)

#include "fetchjobtest.moc"
