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
#include "tasklist.h"
#include "tasklistmodifyjob.h"
#include "types.h"

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

        QTest::newRow("change tasklist") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/tasklist1_modify_request.txt"),
                                                                                                       QFINDTESTDATA("data/tasklist1_modify_response.txt"))}
                                         << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json"))};

        QTest::newRow("batch modify") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/tasklist1_modify_request.txt"),
                                                                                                    QFINDTESTDATA("data/tasklist1_modify_response.txt")),
                                                                                   scenarioFromFile(QFINDTESTDATA("data/tasklist2_modify_request.txt"),
                                                                                                    QFINDTESTDATA("data/tasklist2_modify_response.txt"))}
                                      << TaskListsList{taskListFromFile(QFINDTESTDATA("data/tasklist1.json")),
                                                       taskListFromFile(QFINDTESTDATA("data/tasklist2.json"))};
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
