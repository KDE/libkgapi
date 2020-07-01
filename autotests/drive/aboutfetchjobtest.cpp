/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include <cstring>
#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "drivetestutils.h"

#include "types.h"
#include "aboutfetchjob.h"
#include "about.h"
#include "account.h"

using namespace KGAPI2;

namespace {
    static const char *LimitedFieldTag = "limited fields";
}

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::Drive::AboutPtr)

class AboutFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetch_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<KGAPI2::Drive::AboutPtr>("about");

        QTest::newRow("")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/about_fetch_request.txt"),
                                     QFINDTESTDATA("data/about_fetch_response.txt"))
                }
            << aboutFromFile(QFINDTESTDATA("data/about.json"));

        QTest::newRow(LimitedFieldTag)
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/about_fetch_limited_fields_request.txt"),
                                     QFINDTESTDATA("data/about_fetch_limited_fields_response.txt"))
                }
            << aboutFromFile(QFINDTESTDATA("data/about_limited_fields.json"));
    }

    void testFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(KGAPI2::Drive::AboutPtr, about);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new Drive::AboutFetchJob(account, nullptr);

        if (strcmp(LimitedFieldTag, QTest::currentDataTag()) == 0) {
            job->setFields({ Drive::About::Fields::Kind, Drive::About::Fields::PermissionId});
        }

        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedAbout =  items.at(0).dynamicCast<Drive::About>();
        QVERIFY(returnedAbout);
        QCOMPARE(*returnedAbout, *about);
    }
};

QTEST_GUILESS_MAIN(AboutFetchJobTest)

#include "aboutfetchjobtest.moc"



