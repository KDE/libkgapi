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

