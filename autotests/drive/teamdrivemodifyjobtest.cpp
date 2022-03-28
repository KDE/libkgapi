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
#include "teamdrive.h"
#include "teamdrivemodifyjob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::TeamdrivePtr)

class TeamdriveModifyJobTest : public QObject
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
        QTest::addColumn<Drive::TeamdrivePtr>("sourceTeamdrive");
        QTest::addColumn<QString>("requestId");

        QTest::newRow("metadata only") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/teamdrive_modify_request.txt"),
                                                                                                     QFINDTESTDATA("data/teamdrive_modify_response.txt"))}
                                       << teamdriveFromFile(QFINDTESTDATA("data/teamdrive.json")) << QStringLiteral("MockRequestId");
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(Drive::TeamdrivePtr, sourceTeamdrive);
        QFETCH(QString, requestId);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        sourceTeamdrive->setName(QStringLiteral("Renamed Team Drive"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto *job = new Drive::TeamdriveModifyJob(sourceTeamdrive, account);

        QVERIFY(execJob(job));

        const auto items = job->items();
        QCOMPARE(items.count(), 1);

        Drive::TeamdrivePtr firstResult = (*items.cbegin()).dynamicCast<Drive::Teamdrive>();
        QVERIFY(firstResult);
        QCOMPARE(*firstResult, *sourceTeamdrive);
    }
};

QTEST_GUILESS_MAIN(TeamdriveModifyJobTest)

#include "teamdrivemodifyjobtest.moc"
