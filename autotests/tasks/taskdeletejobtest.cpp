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
#include "taskdeletejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::TasksList)

class TaskDeleteJobTest : public QObject
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
        QTest::addColumn<TasksList>("tasks");
        QTest::addColumn<bool>("uidOnly");

        QTest::newRow("simple task") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/task1_delete_request.txt"),
                                                                                                   QFINDTESTDATA("data/task1_delete_response.txt"))}
                                     << TasksList{taskFromFile(QFINDTESTDATA("data/task1.json"))} << false;

        QTest::newRow("simple task (uid)") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/task1_delete_request.txt"),
                                                                                                         QFINDTESTDATA("data/task1_delete_response.txt"))}
                                           << TasksList{taskFromFile(QFINDTESTDATA("data/task1.json"))} << true;

        QTest::newRow("batch delete") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/task1_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/task1_delete_response.txt")),
                                                                                   scenarioFromFile(QFINDTESTDATA("data/task2_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/task2_delete_response.txt"))}
                                      << TasksList{taskFromFile(QFINDTESTDATA("data/task1.json")), taskFromFile(QFINDTESTDATA("data/task2.json"))} << false;

        QTest::newRow("batch delete (uid)") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/task1_delete_request.txt"),
                                                                                                          QFINDTESTDATA("data/task1_delete_response.txt")),
                                                                                         scenarioFromFile(QFINDTESTDATA("data/task2_delete_request.txt"),
                                                                                                          QFINDTESTDATA("data/task2_delete_response.txt"))}
                                            << TasksList{taskFromFile(QFINDTESTDATA("data/task1.json")), taskFromFile(QFINDTESTDATA("data/task2.json"))}
                                            << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(TasksList, tasks);
        QFETCH(bool, uidOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        TaskDeleteJob *job = nullptr;
        if (tasks.count() == 1) {
            if (uidOnly) {
                job = new TaskDeleteJob(tasks.at(0)->uid(), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new TaskDeleteJob(tasks.at(0), QStringLiteral("MockAccount"), account, nullptr);
            }
        } else {
            if (uidOnly) {
                job = new TaskDeleteJob(elementsToUids(tasks), QStringLiteral("MockAccount"), account, nullptr);
            } else {
                job = new TaskDeleteJob(tasks, QStringLiteral("MockAccount"), account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(TaskDeleteJobTest)

#include "taskdeletejobtest.moc"
