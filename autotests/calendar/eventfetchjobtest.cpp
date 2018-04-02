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
        auto job = new EventFetchJob(event->uid(), QStringLiteral("MockAccount"), account);
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




