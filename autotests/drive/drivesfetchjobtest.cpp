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
#include "drivesfetchjob.h"
#include "drives.h"
#include "account.h"

namespace {
    static const QString MockValue = QStringLiteral("MockValue");
}

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)

class DrivesFetchJobTest : public QObject
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
            scenarioFromFile(QFINDTESTDATA("data/drives_fetch_request.txt"),
                             QFINDTESTDATA("data/drives_fetch_response.txt"))

        });
        const auto drive = drivesFromFile(QFINDTESTDATA("data/drives.json"));

        auto account = AccountPtr::create(MockValue, MockValue);
        auto job = new Drive::DrivesFetchJob(account, nullptr);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 2);
        const auto returnedDrives =  items.at(0).dynamicCast<Drive::Drives>();
        QVERIFY(returnedDrives);
        QCOMPARE(*returnedDrives, *drive);
    }
};

QTEST_GUILESS_MAIN(DrivesFetchJobTest)

#include "drivesfetchjobtest.moc"
