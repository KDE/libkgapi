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
#include "taskfetchjob.h"
#include "task.h"
#include "account.h"

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
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/tasks_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/tasks_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/tasks_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/tasks_fetch_page2_response.txt"))
        });
        const TasksList tasks = {
            taskFromFile(QFINDTESTDATA("data/task1.json")),
            taskFromFile(QFINDTESTDATA("data/task2.json"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TaskFetchJob(QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), tasks.count());
        for (int i = 0; i < tasks.count(); ++i) {
            const auto returnedTask =  items.at(i).dynamicCast<Task>();
            const auto expectedTask = tasks.at(i);
            QVERIFY(returnedTask);
            QCOMPARE(*returnedTask, *expectedTask);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/task1_fetch_request.txt"),
                             QFINDTESTDATA("data/task1_fetch_response.txt"))

        });
        const auto task = taskFromFile(QFINDTESTDATA("data/task1.json"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TaskFetchJob(task->uid(), QStringLiteral("MockAccount"), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedTask =  items.at(0).dynamicCast<Task>();
        QVERIFY(returnedTask);
        QCOMPARE(*returnedTask, *task);
    }
};

QTEST_GUILESS_MAIN(TaskFetchJobTest)

#include "taskfetchjobtest.moc"



