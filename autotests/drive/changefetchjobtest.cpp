/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "drivetestutils.h"

#include "types.h"
#include "changefetchjob.h"
#include "change.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::ChangesList)

class ChangeFetchJobTest : public QObject
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
            scenarioFromFile(QFINDTESTDATA("data/changes_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/changes_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/changes_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/changes_fetch_page2_response.txt"))
        });
        const Drive::ChangesList changes = {
            changeFromFile(QFINDTESTDATA("data/change1.json")),
            changeFromFile(QFINDTESTDATA("data/change2.json"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new Drive::ChangeFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), changes.count());
        for (int i = 0; i < changes.count(); ++i) {
            const auto returnedChange =  items.at(i).dynamicCast<Drive::Change>();
            const auto expectedChange = changes.at(i);
            QVERIFY(returnedChange);
            QCOMPARE(*returnedChange, *expectedChange);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/change1_fetch_request.txt"),
                             QFINDTESTDATA("data/change1_fetch_response.txt"))

        });
        const auto change = changeFromFile(QFINDTESTDATA("data/change1.json"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new Drive::ChangeFetchJob(QString::number(change->id()), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedChange =  items.at(0).dynamicCast<Drive::Change>();
        QVERIFY(returnedChange);
        QCOMPARE(*returnedChange, *change);
    }
};

QTEST_GUILESS_MAIN(ChangeFetchJobTest)

#include "changefetchjobtest.moc"





