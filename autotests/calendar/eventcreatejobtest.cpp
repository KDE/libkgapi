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
#include "event.h"
#include "eventcreatejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::EventsList)

class EventCreateJobTest : public QObject
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
        QTest::addColumn<EventsList>("events");
        QTest::addColumn<EventsList>("responses");
        QTest::addColumn<QString>("calendarId");

        auto event1 = eventFromFile(QFINDTESTDATA("data/event1.json"));
        auto response1 = EventPtr::create(*event1);
        QTest::newRow("simple event") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/event1_create_request.txt"),
                                                                                                    QFINDTESTDATA("data/event1_create_response.txt"))}
                                      << EventsList{event1} << EventsList{response1} << QStringLiteral("MockAccount");

        // The calendar owner is already an attendee of the imported event, so no attendee must be added
        QTest::newRow("import, owner is attendee") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(
            QFINDTESTDATA("data/event1_import_owner_attendee_request.txt"),
            QFINDTESTDATA("data/event1_create_response.txt"))}
                                                   << EventsList{event1} << EventsList{response1} << QStringLiteral("attendee1@kde.test");

        auto event2 = eventFromFile(QFINDTESTDATA("data/event2.json"));
        auto response2 = EventPtr::create(*event2);
        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_create_request.txt"),
                                     QFINDTESTDATA("data/event1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/event2_create_request.txt"),
                                     QFINDTESTDATA("data/event2_create_response.txt")),
                }
            << EventsList{ event1, event2 }
            << EventsList{ response1, response2 }
            << QStringLiteral("MockAccount");
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(EventsList, events);
        QFETCH(EventsList, responses);
        QFETCH(QString, calendarId);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        EventCreateJob *job = nullptr;
        if (events.count() == 1) {
            job = new EventCreateJob(events.at(0), calendarId, account);
        } else {
            job = new EventCreateJob(events, calendarId, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), responses.count());
        for (int i = 0; i < responses.count(); ++i) {
            const auto returnedEvent = items.at(i).dynamicCast<Event>();
            QVERIFY(returnedEvent);
            QCOMPARE(*returnedEvent, *responses.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(EventCreateJobTest)

#include "eventcreatejobtest.moc"
