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

