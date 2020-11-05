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
#include "drivescreatejob.h"
#include "drives.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::DrivesPtr)

class DrivesCreateJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testCreate_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<Drive::DrivesPtr>("sourceDrives");
        QTest::addColumn<QString>("requestId");
        QTest::addColumn<Drive::DrivesPtr>("expectedResult");

        QTest::newRow("metadata only")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/drives_create_request.txt"),
                                     QFINDTESTDATA("data/drives_create_response.txt"))
                }
            << drivesFromFile(QFINDTESTDATA("data/drives.json"))
            << QStringLiteral("MockRequestId")
            << drivesFromFile(QFINDTESTDATA("data/drives.json"));
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::DrivesPtr, sourceDrives);
        QFETCH(QString, requestId);
        QFETCH(Drive::DrivesPtr, expectedResult);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto *job = new Drive::DrivesCreateJob(requestId, sourceDrives, account);

        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        QVERIFY(*items.cbegin());
        QCOMPARE(**items.cbegin(), *expectedResult);
        QCOMPARE(requestId, job->requestId());
    }
};

QTEST_GUILESS_MAIN(DrivesCreateJobTest)

#include "drivescreatejobtest.moc"





