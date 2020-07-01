/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "calendartestutils.h"

#include "types.h"
#include "freebusyqueryjob.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::FreeBusyQueryJob::BusyRangeList)

class FreeBusyQueryJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testQuery_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<QDateTime>("rangeStart");
        QTest::addColumn<QDateTime>("rangeEnd");
        QTest::addColumn<FreeBusyQueryJob::BusyRangeList>("ranges");

        QTest::newRow("query")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/freebusy1_query_request.txt"),
                                     QFINDTESTDATA("data/freebusy1_query_response.txt")),
                }
            << QDateTime({ 2018, 4, 1 }, { 10, 0, 0 }, QTimeZone("Europe/Prague"))
            << QDateTime({ 2018, 4, 2 }, { 16, 0, 0 }, QTimeZone("Europe/Prague"))
            << FreeBusyQueryJob::BusyRangeList{
                { { { 2018, 4, 1 }, { 12, 30, 0 }, QTimeZone("Europe/Prague") },
                  { { 2018, 4, 1 }, { 13, 30, 0 }, QTimeZone("Europe/Prague") } },
                { { { 2018, 4, 1 }, { 15, 0, 0 }, QTimeZone("Europe/Prague") },
                  { { 2018, 4, 1 }, { 16, 30, 0 }, QTimeZone("Europe/Prague") } }
            };
    }

    void testQuery()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QDateTime, rangeStart);
        QFETCH(QDateTime, rangeEnd);
        QFETCH(FreeBusyQueryJob::BusyRangeList, ranges);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new FreeBusyQueryJob(QStringLiteral("MockAccount"), rangeStart, rangeEnd, account);
        QVERIFY(execJob(job));
        const auto items = job->busy();
        QCOMPARE(items.count(), ranges.count());
        for (int i = 0; i < ranges.count(); ++i) {
            const auto returnedFreeBusy = items.at(i);
            QCOMPARE(returnedFreeBusy, ranges.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(FreeBusyQueryJobTest)

#include "freebusyqueryjobtest.moc"



