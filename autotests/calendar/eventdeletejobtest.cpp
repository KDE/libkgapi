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
        QTest::addColumn<bool>("idOnly");

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
        QFETCH(bool, idOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        EventDeleteJob *job = nullptr;
        if (events.count() == 1) {
            if (idOnly) {
                job = new EventDeleteJob(events.at(0)->id(), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new EventDeleteJob(events.at(0), QStringLiteral("MockAccount"), account, nullptr);
            }
        } else {
            if (idOnly) {
                job = new EventDeleteJob(elementsToIds(events), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new EventDeleteJob(events, QStringLiteral("MockAccount"), account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(EventDeleteJobTest)

#include "eventdeletejobtest.moc"



