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
#include "eventfetchjob.h"
#include "event.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::EventsList)

class EventFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetchAll()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/events_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/events_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/events_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/events_fetch_page2_response.txt"))
        });
        const EventsList events = {
            eventFromFile(QFINDTESTDATA("data/event1.json")),
            eventFromFile(QFINDTESTDATA("data/event2.json"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new EventFetchJob(QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), events.count());
        for (int i = 0; i < events.count(); ++i) {
            const auto returnedEvent =  items.at(i).dynamicCast<Event>();
            const auto expectedEvent = events.at(i);
            QVERIFY(returnedEvent);
            QCOMPARE(*returnedEvent, *expectedEvent);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/event1_fetch_request.txt"),
                             QFINDTESTDATA("data/event1_fetch_response.txt"))

        });
        const auto event = eventFromFile(QFINDTESTDATA("data/event1.json"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new EventFetchJob(event->id(), QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedEvent =  items.at(0).dynamicCast<Event>();
        QVERIFY(returnedEvent);
        QCOMPARE(*returnedEvent, *event);
    }
};

QTEST_GUILESS_MAIN(EventFetchJobTest)

#include "eventfetchjobtest.moc"




