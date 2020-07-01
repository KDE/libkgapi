/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * move it under the terms of the GNU Lesser General Public
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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "taskstestutils.h"

#include "types.h"
#include "taskmovejob.h"
#include "task.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TasksList)

class TaskMoveJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testMove_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<TasksList>("tasks");
        QTest::addColumn<QString>("newParent");

        QTest::newRow("single move to top-level")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_move_request.txt"),
                                     QFINDTESTDATA("data/task1_move_response.txt")),
                }
            << TasksList{ taskFromFile(QFINDTESTDATA("data/task1.json")) }
            << QString();

        QTest::newRow("single move to new parent")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task2_move_with_parent_request.txt"),
                                     QFINDTESTDATA("data/task2_move_response.txt")),
                }
            << TasksList{ taskFromFile(QFINDTESTDATA("data/task2.json")) }
            << QStringLiteral("newMockParent");

        QTest::newRow("batch move")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/task1_move_request.txt"),
                                     QFINDTESTDATA("data/task1_move_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/task2_move_request.txt"),
                                     QFINDTESTDATA("data/task2_move_response.txt"))
                }
            << TasksList{
                    taskFromFile(QFINDTESTDATA("data/task1.json")),
                    taskFromFile(QFINDTESTDATA("data/task2.json"))
                }
            << QString();
    }

    void testMove()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TasksList, tasks);
        QFETCH(QString, newParent);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskMoveJob *job = nullptr;
        if (tasks.count() == 1) {
            job = new TaskMoveJob(tasks.at(0), QStringLiteral("MockAccount"), newParent, account);
        } else {
            job = new TaskMoveJob(tasks, QStringLiteral("MockAccount"), newParent, account);
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(TaskMoveJobTest)

#include "taskmovejobtest.moc"



