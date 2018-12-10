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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "taskstestutils.h"

#include "types.h"
#include "tasklistdeletejob.h"
#include "tasklist.h"
#include "account.h"

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

        QTest::newRow("simple tasklist")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_delete_response.txt"))
                }
            << TaskListsList{ taskListFromFile(QFINDTESTDATA("data/tasklist1.json")) }
            << false;

        QTest::newRow("simple tasklist (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_delete_response.txt"))
                }
            << TaskListsList{ taskListFromFile(QFINDTESTDATA("data/tasklist1.json")) }
            << true;

        QTest::newRow("batch delete")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/tasklist2_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist2_delete_response.txt"))
                }
            << TaskListsList{
                    taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                    taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))
                }
            << false;


        QTest::newRow("batch delete (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/tasklist2_delete_request.txt"),
                                     QFINDTESTDATA("data/tasklist2_delete_response.txt"))
                }
            << TaskListsList{
                    taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                    taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))
                }
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


