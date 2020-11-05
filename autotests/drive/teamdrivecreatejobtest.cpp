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
#include "teamdrivecreatejob.h"
#include "teamdrive.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::TeamdrivePtr)

class TeamdriveCreateJobTest : public QObject
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
        QTest::addColumn<Drive::TeamdrivePtr>("sourceTeamdrive");
        QTest::addColumn<QString>("requestId");
        QTest::addColumn<Drive::TeamdrivePtr>("expectedResult");

        QTest::newRow("metadata only")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/teamdrive_create_request.txt"),
                                     QFINDTESTDATA("data/teamdrive_create_response.txt"))
                }
            << teamdriveFromFile(QFINDTESTDATA("data/teamdrive.json"))
            << QStringLiteral("MockRequestId")
            << teamdriveFromFile(QFINDTESTDATA("data/teamdrive.json"));
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::TeamdrivePtr, sourceTeamdrive);
        QFETCH(QString, requestId);
        QFETCH(Drive::TeamdrivePtr, expectedResult);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto *job = new Drive::TeamdriveCreateJob(requestId, sourceTeamdrive, account);

        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        QVERIFY(*items.cbegin());
        QCOMPARE(**items.cbegin(), *expectedResult);
        QCOMPARE(requestId, job->requestId());
    }
};

QTEST_GUILESS_MAIN(TeamdriveCreateJobTest)

#include "teamdrivecreatejobtest.moc"





