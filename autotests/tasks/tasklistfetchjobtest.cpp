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
#include "tasklistfetchjob.h"
#include "tasklist.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TaskListPtr)

class TaskListFetchJobTest : public QObject
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
            scenarioFromFile(QFINDTESTDATA("data/tasklists_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/tasklists_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/tasklists_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/tasklists_fetch_page2_response.txt"))
        });
        const TaskListsList taskLists = {
            taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
            taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new TaskListFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), taskLists.count());
        for (int i = 0; i < taskLists.count(); ++i) {
            const auto returnedGroup =  items.at(i).dynamicCast<TaskList>();
            QVERIFY(returnedGroup);
            QCOMPARE(*returnedGroup, *taskLists.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(TaskListFetchJobTest)

#include "tasklistfetchjobtest.moc"

