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


