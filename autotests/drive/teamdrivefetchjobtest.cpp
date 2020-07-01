/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "drivetestutils.h"

#include "types.h"
#include "teamdrivefetchjob.h"
#include "teamdrive.h"
#include "account.h"

namespace {
    static const QString MockValue = QStringLiteral("MockValue");
}

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)

class TeamdriveFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetch()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/teamdrive_fetch_request.txt"),
                             QFINDTESTDATA("data/teamdrive_fetch_response.txt"))

        });
        const auto teamdrive = teamdriveFromFile(QFINDTESTDATA("data/teamdrive.json"));

        auto account = AccountPtr::create(MockValue, MockValue);
        auto job = new Drive::TeamdriveFetchJob(account, nullptr);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 2);
        const auto returnedTeamdrive =  items.at(0).dynamicCast<Drive::Teamdrive>();
        QVERIFY(returnedTeamdrive);
        QCOMPARE(*returnedTeamdrive, *teamdrive);
    }
};

QTEST_GUILESS_MAIN(TeamdriveFetchJobTest)

#include "teamdrivefetchjobtest.moc"



