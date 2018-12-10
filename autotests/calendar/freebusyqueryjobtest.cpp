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



