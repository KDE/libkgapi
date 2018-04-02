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
#include "eventdeletejob.h"
#include "event.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::EventsList)

class EventDeleteJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testDelete_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<EventsList>("events");
        QTest::addColumn<bool>("uidOnly");

        QTest::newRow("simple event")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_delete_request.txt"),
                                     QFINDTESTDATA("data/event1_delete_response.txt"))
                }
            << EventsList{ eventFromFile(QFINDTESTDATA("data/event1.json")) }
            << false;

        QTest::newRow("simple event (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_delete_request.txt"),
                                     QFINDTESTDATA("data/event1_delete_response.txt"))
                }
            << EventsList{ eventFromFile(QFINDTESTDATA("data/event1.json")) }
            << true;

        QTest::newRow("batch delete")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_delete_request.txt"),
                                     QFINDTESTDATA("data/event1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/event2_delete_request.txt"),
                                     QFINDTESTDATA("data/event2_delete_response.txt"))
                }
            << EventsList{
                    eventFromFile(QFINDTESTDATA("data/event1.json")),
                    eventFromFile(QFINDTESTDATA("data/event2.json"))
                }
            << false;

        QTest::newRow("batch delete (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/event1_delete_request.txt"),
                                     QFINDTESTDATA("data/event1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/event2_delete_request.txt"),
                                     QFINDTESTDATA("data/event2_delete_response.txt"))
                }
            << EventsList{
                    eventFromFile(QFINDTESTDATA("data/event1.json")),
                    eventFromFile(QFINDTESTDATA("data/event2.json"))
                }
            << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(EventsList, events);
        QFETCH(bool, uidOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        EventDeleteJob *job = nullptr;
        if (events.count() == 1) {
            if (uidOnly) {
                job = new EventDeleteJob(events.at(0)->uid(), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new EventDeleteJob(events.at(0), QStringLiteral("MockAccount"), account, nullptr);
            }
        } else {
            if (uidOnly) {
                job = new EventDeleteJob(elementsToUids(events), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new EventDeleteJob(events, QStringLiteral("MockAccount"), account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(EventDeleteJobTest)

#include "eventdeletejobtest.moc"



