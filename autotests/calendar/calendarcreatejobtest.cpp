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
#include "calendarcreatejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::CalendarsList)

class CalendarCreateJobTest : public QObject
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
        QTest::addColumn<CalendarsList>("calendars");
        QTest::addColumn<CalendarsList>("responses");

        auto calendar1 = calendarFromFile(QFINDTESTDATA("data/calendar1.json"));
        auto response1 = CalendarPtr::create();
        response1->setUid(calendar1->uid());
        response1->setTitle(calendar1->title());
        response1->setDetails(calendar1->details());
        response1->setTimezone(calendar1->timezone());
        response1->setEtag(calendar1->etag());

        calendar1->setUid({});
        QTest::newRow("simple calendar") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/calendar1_create_request.txt"),
                                                                                                       QFINDTESTDATA("data/calendar1_create_response.txt"))}
                                         << CalendarsList{calendar1} << CalendarsList{response1};

        auto calendar2 = calendarFromFile(QFINDTESTDATA("data/calendar2.json"));
        auto response2 = CalendarPtr::create();
        response2->setUid(calendar2->uid());
        response2->setTitle(calendar2->title());
        response2->setDetails(calendar2->details());
        response2->setTimezone(calendar2->timezone());
        response2->setEtag(calendar2->etag());

        calendar2->setUid({});
        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_create_request.txt"),
                                     QFINDTESTDATA("data/calendar1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/calendar2_create_request.txt"),
                                     QFINDTESTDATA("data/calendar2_create_response.txt")),
                }
            << CalendarsList{ calendar1, calendar2 }
            << CalendarsList{ response1, response2 };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(CalendarsList, calendars);
        QFETCH(CalendarsList, responses);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        CalendarCreateJob *job = nullptr;
        if (calendars.count() == 1) {
            job = new CalendarCreateJob(calendars.at(0), account);
        } else {
            job = new CalendarCreateJob(calendars, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), responses.count());
        for (int i = 0; i < responses.count(); ++i) {
            const auto returnedCalendar = items.at(i).dynamicCast<Calendar>();
            QVERIFY(returnedCalendar);
            QCOMPARE(*returnedCalendar, *responses.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(CalendarCreateJobTest)

#include "calendarcreatejobtest.moc"
