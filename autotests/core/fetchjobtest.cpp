/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
        QNetworkRequest request(mUrl);
        enqueueRequest(request);
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
            { QUrl(QStringLiteral("https://example.test/request/data?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, 200, "Test Response", false }
        };

        QTest::newRow("not found") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/does/not/exist?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::NotFound, {}, false }
        };

        QTest::newRow("redirect") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::TemporarilyMoved, "https://example.test/moved/data", false },
            { QUrl(QStringLiteral("https://example.test/moved/data?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::OK, "Here's your data", false }
        };
    }

    void testUnauthenticatedFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto job = new TestFetchJob(scenarios.first().requestUrl);
        QVERIFY(execJob(job));
        QCOMPARE(static_cast<int>(job->error()), scenarios.last().responseCode == 200 ? KGAPI2::NoError : scenarios.last().responseCode);
        QCOMPARE(job->response(), scenarios.last().responseData);

        QVERIFY(!FakeNetworkAccessManagerFactory::get()->hasScenario());
    }

    void testAuthenticatedFetch_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");

        QTest::newRow("success") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, 200, "Response" }
        };

        QTest::newRow("token expired") << Scenarios{
            { QUrl(QStringLiteral("https://example.test/request/data?prettyPrint=false")), QNetworkAccessManager::GetOperation,
              {}, KGAPI2::Unauthorized, {} }
        };
    }

    void testAuthenticatedFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TestFetchJob(account, scenarios.first().requestUrl);
        QVERIFY(execJob(job));
        QCOMPARE(static_cast<int>(job->error()), scenarios.last().responseCode == 200 ? KGAPI2::NoError : scenarios.last().responseCode);
        QCOMPARE(job->response(), scenarios.last().responseData);

        QVERIFY(!FakeNetworkAccessManagerFactory::get()->hasScenario());
    }
};

QTEST_GUILESS_MAIN(FetchJobTest)

#include "fetchjobtest.moc"
