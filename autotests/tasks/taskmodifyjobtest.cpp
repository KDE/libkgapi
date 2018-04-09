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
#include "taskmodifyjob.h"
#include "task.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TasksList)

class TaskModifyJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testModify_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<TasksList>("tasks");

        QTest::newRow("change task")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_modify_request.txt"),
                                     QFINDTESTDATA("data/task1_modify_response.txt")),
                }
            << TasksList{ taskFromFile(QFINDTESTDATA("data/task1.json")) };

        QTest::newRow("batch modify")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_modify_request.txt"),
                                     QFINDTESTDATA("data/task1_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/task2_modify_request.txt"),
                                     QFINDTESTDATA("data/task2_modify_response.txt"))
                }
            << TasksList{
                    taskFromFile(QFINDTESTDATA("data/task1.json")),
                    taskFromFile(QFINDTESTDATA("data/task2.json"))
                };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TasksList, tasks);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskModifyJob *job = nullptr;
        if (tasks.count() == 1) {
            job = new TaskModifyJob(tasks.at(0), QStringLiteral("MockAccount"), account);
        } else {
            job = new TaskModifyJob(tasks, QStringLiteral("MockAccount"), account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), tasks.count());
        for (int i = 0; i < tasks.count(); ++i) {
            const auto returnedTask =  items.at(i).dynamicCast<Task>();
            QVERIFY(returnedTask);
            QCOMPARE(*returnedTask, *tasks.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(TaskModifyJobTest)

#include "taskmodifyjobtest.moc"


