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
#include "eventcreatejob.h"
#include "event.h"
#include "account.h"

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

        auto event1 = eventFromFile(QFINDTESTDATA("data/event1.json"));
        auto response1 = EventPtr::create(*event1);
        event1->setUid({});
        QTest::newRow("simple event")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_create_request.txt"),
                                     QFINDTESTDATA("data/event1_create_response.txt"))
                }
            << EventsList{ event1 }
            << EventsList{ response1 };

        auto event2 = eventFromFile(QFINDTESTDATA("data/event2.json"));
        auto response2 = EventPtr::create(*event2);
        event2->setUid({});
        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_create_request.txt"),
                                     QFINDTESTDATA("data/event1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/event2_create_request.txt"),
                                     QFINDTESTDATA("data/event2_create_response.txt")),
                }
            << EventsList{ event1, event2 }
            << EventsList{ response1, response2 };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(EventsList, events);
        QFETCH(EventsList, responses);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        EventCreateJob *job = nullptr;
        if (events.count() == 1) {
            job = new EventCreateJob(events.at(0), QStringLiteral("MockAccount"), account);
        } else {
            job = new EventCreateJob(events, QStringLiteral("MockAccount"), account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), responses.count());
        for (int i = 0; i < responses.count(); ++i) {
            const auto returnedEvent =  items.at(i).dynamicCast<Event>();
            QVERIFY(returnedEvent);
            QCOMPARE(*returnedEvent, *responses.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(EventCreateJobTest)

#include "eventcreatejobtest.moc"



