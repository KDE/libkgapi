/*
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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
        Drive::DrivesCreateJob *job = new Drive::DrivesCreateJob(requestId, sourceDrives, account);

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





