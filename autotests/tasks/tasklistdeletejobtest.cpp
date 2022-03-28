/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "taskstestutils.h"
#include "testutils.h"

#include "account.h"
#include "tasklist.h"
#include "tasklistdeletejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TaskListsList)

class TaskListDeleteJobTest : public QObject
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
        QTest::addColumn<TaskListsList>("tasklists");
        QTest::addColumn<bool>("uidOnly");

        QTest::newRow("simple tasklist") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                                                                                       QFINDTESTDATA("data/tasklist1_delete_response.txt"))}
                                         << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json"))} << false;

        QTest::newRow("simple tasklist (uid)") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(
            QFINDTESTDATA("data/tasklist1_delete_request.txt"),
            QFINDTESTDATA("data/tasklist1_delete_response.txt"))}
                                               << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json"))} << true;

        QTest::newRow("batch delete") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/tasklist1_delete_response.txt")),
                                                                                   scenarioFromFile(QFINDTESTDATA("data/tasklist2_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/tasklist2_delete_response.txt"))}
                                      << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                                                       taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))}
                                      << false;

        QTest::newRow("batch delete (uid)") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                                                                                          QFINDTESTDATA("data/tasklist1_delete_response.txt")),
                                                                                         scenarioFromFile(QFINDTESTDATA("data/tasklist2_delete_request.txt"),
                                                                                                          QFINDTESTDATA("data/tasklist2_delete_response.txt"))}
                                            << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                                                             taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))}
                                            << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TaskListsList, tasklists);
        QFETCH(bool, uidOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskListDeleteJob *job = nullptr;
        if (tasklists.count() == 1) {
            if (uidOnly) {
                job = new TaskListDeleteJob(tasklists.at(0)->uid(), account, nullptr);
            } else {
                job = new TaskListDeleteJob(tasklists.at(0), account, nullptr);
            }
        } else {
            if (uidOnly) {
                job = new TaskListDeleteJob(elementsToUids(tasklists), account, nullptr);
            } else {
                job = new TaskListDeleteJob(tasklists, account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(TaskListDeleteJobTest)

#include "tasklistdeletejobtest.moc"
