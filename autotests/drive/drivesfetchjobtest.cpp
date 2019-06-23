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
