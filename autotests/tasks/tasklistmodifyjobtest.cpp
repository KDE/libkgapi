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
#include "tasklistmodifyjob.h"
#include "tasklist.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TaskListsList)

class TaskListModifyJobTest : public QObject
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
        QTest::addColumn<TaskListsList>("tasklists");

        QTest::newRow("change tasklist")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_modify_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_modify_response.txt"))
                }
            << TaskListsList{ taskListFromFile(QFINDTESTDATA("data/tasklist1.json")) };

        QTest::newRow("batch modify")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/tasklist1_modify_request.txt"),
                                     QFINDTESTDATA("data/tasklist1_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/tasklist2_modify_request.txt"),
                                     QFINDTESTDATA("data/tasklist2_modify_response.txt"))
                }
            << TaskListsList{
                    taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                    taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))
                };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TaskListsList, tasklists);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskListModifyJob *job = nullptr;
        if (tasklists.count() == 1) {
            job = new TaskListModifyJob(tasklists.at(0), account);
        } else {
            job = new TaskListModifyJob(tasklists, account);
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(TaskListModifyJobTest)

#include "tasklistmodifyjobtest.moc"


