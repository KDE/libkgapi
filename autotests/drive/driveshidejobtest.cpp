/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "drivetestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "drives.h"
#include "driveshidejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::DrivesPtr)

class DrivesHideJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testHide_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<Drive::DrivesPtr>("sourceDrive");
        QTest::addColumn<Drive::DrivesPtr>("expectedResult");

        QTest::newRow("metadata only") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/drives_hide_request.txt"),
                                                                                                     QFINDTESTDATA("data/drives_hide_response.txt"))}
                                       << drivesFromFile(QFINDTESTDATA("data/drives.json")) << drivesFromFile(QFINDTESTDATA("data/drives.json"));
    }

    void testHide()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::DrivesPtr, sourceDrive);
        QFETCH(Drive::DrivesPtr, expectedResult);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new Drive::DrivesHideJob(sourceDrive, true, account);

        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        QVERIFY(*items.cbegin());
        QCOMPARE(**items.cbegin(), *expectedResult);
    }
};

QTEST_GUILESS_MAIN(DrivesHideJobTest)

#include "driveshidejobtest.moc"
