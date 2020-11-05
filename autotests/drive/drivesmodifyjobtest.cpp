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
#include "drivesmodifyjob.h"
#include "drives.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::DrivesPtr)

class DrivesModifyJobTest : public QObject
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
        QTest::addColumn<Drive::DrivesPtr>("sourceDrives");
        QTest::addColumn<QString>("requestId");

        QTest::newRow("metadata only")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/drives_modify_request.txt"),
                                     QFINDTESTDATA("data/drives_modify_response.txt"))
                }
            << drivesFromFile(QFINDTESTDATA("data/drives.json"))
            << QStringLiteral("MockRequestId");
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::DrivesPtr, sourceDrives);
        QFETCH(QString, requestId);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        sourceDrives->setName(QStringLiteral("Renamed Drive"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto *job = new Drive::DrivesModifyJob(sourceDrives, account);

        QVERIFY(execJob(job));

        const auto items = job->items();
        QCOMPARE(items.count(), 1);

        Drive::DrivesPtr firstResult = (*items.cbegin()).dynamicCast<Drive::Drives>();
        QVERIFY(firstResult);
        QCOMPARE(*firstResult, *sourceDrives);
    }
};

QTEST_GUILESS_MAIN(DrivesModifyJobTest)

#include "drivesmodifyjobtest.moc"





