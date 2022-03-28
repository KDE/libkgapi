/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "calendartestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "calendar.h"
#include "calendarfetchjob.h"
#include "types.h"

#include <iostream>

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::CalendarsList)

class CalendarFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetchAll()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            {scenarioFromFile(QFINDTESTDATA("data/calendars_fetch_page1_request.txt"), QFINDTESTDATA("data/calendars_fetch_page1_response.txt")),
             scenarioFromFile(QFINDTESTDATA("data/calendars_fetch_page2_request.txt"), QFINDTESTDATA("data/calendars_fetch_page2_response.txt"))});
        const CalendarsList calendars = {calendarFromFile(QFINDTESTDATA("data/calendar1.json")), calendarFromFile(QFINDTESTDATA("data/calendar2.json"))};

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new CalendarFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), calendars.count());
        for (int i = 0; i < calendars.count(); ++i) {
            const auto returnedCalendar = items.at(i).dynamicCast<Calendar>();
            const auto expectedCalendar = calendars.at(i);
            QVERIFY(returnedCalendar);
            QCOMPARE(*returnedCalendar, *expectedCalendar);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            {scenarioFromFile(QFINDTESTDATA("data/calendar1_fetch_request.txt"), QFINDTESTDATA("data/calendar1_fetch_response.txt"))

            });
        const auto calendar = calendarFromFile(QFINDTESTDATA("data/calendar1.json"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new CalendarFetchJob(calendar->uid(), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedCalendar = items.at(0).dynamicCast<Calendar>();
        QVERIFY(returnedCalendar);
        QCOMPARE(*returnedCalendar, *calendar);
    }
};

QTEST_GUILESS_MAIN(CalendarFetchJobTest)

#include "calendarfetchjobtest.moc"
