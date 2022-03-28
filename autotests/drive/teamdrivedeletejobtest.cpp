/*
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "teamdrive.h"
#include "teamdrivedeletejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::TeamdrivePtr)

class TeamdriveDeleteJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testDelete_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<QString>("teamdriveId");

        QTest::newRow("metadata only") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/teamdrive_delete_request.txt"),
                                                                                                     QFINDTESTDATA("data/generic_no_content_response.txt"))}
                                       << QStringLiteral("somelongid");
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QString, teamdriveId);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto *job = new Drive::TeamdriveDeleteJob(teamdriveId, account);

        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(TeamdriveDeleteJobTest)

#include "teamdrivedeletejobtest.moc"
