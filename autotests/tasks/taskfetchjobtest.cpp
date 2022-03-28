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
#include "task.h"
#include "taskfetchjob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TasksList)

class TaskFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetchAll()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            {scenarioFromFile(QFINDTESTDATA("data/tasks_fetch_page1_request.txt"), QFINDTESTDATA("data/tasks_fetch_page1_response.txt")),
             scenarioFromFile(QFINDTESTDATA("data/tasks_fetch_page2_request.txt"), QFINDTESTDATA("data/tasks_fetch_page2_response.txt"))});
        const TasksList tasks = {taskFromFile(QFINDTESTDATA("data/task1.json")), taskFromFile(QFINDTESTDATA("data/task2.json"))};

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TaskFetchJob(QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), tasks.count());
        for (int i = 0; i < tasks.count(); ++i) {
            const auto returnedTask = items.at(i).dynamicCast<Task>();
            const auto expectedTask = tasks.at(i);
            QVERIFY(returnedTask);
            QCOMPARE(*returnedTask, *expectedTask);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios(
            {scenarioFromFile(QFINDTESTDATA("data/task1_fetch_request.txt"), QFINDTESTDATA("data/task1_fetch_response.txt"))

            });
        const auto task = taskFromFile(QFINDTESTDATA("data/task1.json"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TaskFetchJob(task->uid(), QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedTask = items.at(0).dynamicCast<Task>();
        QVERIFY(returnedTask);
        QCOMPARE(*returnedTask, *task);
    }
};

QTEST_GUILESS_MAIN(TaskFetchJobTest)

#include "taskfetchjobtest.moc"
