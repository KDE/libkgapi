/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "taskstestutils.h"

#include "types.h"
#include "tasklistcreatejob.h"
#include "tasklist.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TaskListsList)

class TaskListCreateJobTest : public QObject
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
        QTest::addColumn<TaskListsList>("taskLists");
        QTest::addColumn<TaskListsList>("expectedTaskLists");

        auto taskList1 = taskListFromFile(QFINDTESTDATA("data/tasklist1.json"));
        taskList1->setUid({});
        QTest::newRow("simple tasklist")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_create_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_create_response.txt"))
                }
            << TaskListsList{ taskList1 }
            << TaskListsList{ taskListFromFile(QFINDTESTDATA("data/tasklist1.json")) };

        auto taskList2 = taskListFromFile(QFINDTESTDATA("data/tasklist2.json"));
        taskList2->setUid({});
        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_create_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/tasklist2_create_request.txt"),
                                     QFINDTESTDATA("data/tasklist2_create_response.txt"))
                }
            << TaskListsList{ taskList1, taskList2 }
            << TaskListsList{ taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                              taskListFromFile(QFINDTESTDATA("data/tasklist2.json")) };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TaskListsList, taskLists);
        QFETCH(TaskListsList, expectedTaskLists);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskListCreateJob *job = nullptr;
        if (taskLists.count() == 1) {
            job = new TaskListCreateJob(taskLists.at(0), account);
        } else {
            job = new TaskListCreateJob(taskLists, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), expectedTaskLists.count());
        for (int i = 0; i < expectedTaskLists.count(); ++i) {
            const auto returnedList =  items.at(i).dynamicCast<TaskList>();
            QVERIFY(returnedList);
            QCOMPARE(*returnedList, *expectedTaskLists.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(TaskListCreateJobTest)

#include "tasklistcreatejobtest.moc"

