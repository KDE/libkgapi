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
#include "taskcreatejob.h"
#include "task.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TasksList)

class TaskCreateJobTest : public QObject
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
        QTest::addColumn<TasksList>("tasks");
        QTest::addColumn<TasksList>("expected");

        auto task1 = taskFromFile(QFINDTESTDATA("data/task1.json"));
        task1->setUid({});
        QTest::newRow("simple task")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_create_request.txt"),
                                     QFINDTESTDATA("data/task1_create_response.txt"))
                }
            << TasksList{ task1 }
            << TasksList{ taskFromFile(QFINDTESTDATA("data/task1.json")) };

        auto task2 = taskFromFile(QFINDTESTDATA("data/task2.json"));
        task2->setUid({});
        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_create_request.txt"),
                                     QFINDTESTDATA("data/task1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/task2_create_request.txt"),
                                     QFINDTESTDATA("data/task2_create_response.txt"))
                }
            << TasksList{ task1, task2 }
            << TasksList{ taskFromFile(QFINDTESTDATA("data/task1.json")),
                          taskFromFile(QFINDTESTDATA("data/task2.json")) };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TasksList, tasks);
        QFETCH(TasksList, expected);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskCreateJob *job = nullptr;
        if (tasks.count() == 1) {
            job = new TaskCreateJob(tasks.at(0), QStringLiteral("MockAccount"), account);
        } else {
            job = new TaskCreateJob(tasks, QStringLiteral("MockAccount"), account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), expected.count());
        for (int i = 0; i < expected.count(); ++i) {
            const auto returnedTask =  items.at(i).dynamicCast<Task>();
            QVERIFY(returnedTask);
            QCOMPARE(*returnedTask, *expected.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(TaskCreateJobTest)

#include "taskcreatejobtest.moc"


