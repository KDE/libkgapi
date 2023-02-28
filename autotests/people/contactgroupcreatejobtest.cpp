/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include <KContacts/Picture>

#include "peopletestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "types.h"
#include "people/contactgroup.h"
#include "people/contactgroupcreatejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::ContactGroupList)

namespace KGAPI2 {
namespace People {

class ContactGroupCreateJobTest : public QObject
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
        QTest::addColumn<ContactGroupList>("contactGroupCreateData");
        QTest::addColumn<ContactGroupList>("contactGroupCreated");

        const auto contactGroup2CreateData = TestUtils::contactGroupFromRequestDataFile(QFINDTESTDATA("data/contactgroup2_create_data.json"));
        const auto contactGroup3CreateData = TestUtils::contactGroupFromRequestDataFile(QFINDTESTDATA("data/contactgroup3_create_data.json"));
        const auto contactGroup2CreateRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup2_create_request.txt"), QFINDTESTDATA("data/contactgroup2_create_response.txt"));
        const auto contactGroup3CreateRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup3_create_request.txt"), QFINDTESTDATA("data/contactgroup3_create_response.txt"));
        const auto contactGroup2Created = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup2.json"));
        const auto contactGroup3Created = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup3.json"));

        qDebug() << contactGroup2CreateData->name() << contactGroup3CreateData->name();
        QTest::newRow("simple contactGroup2") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2CreateRequest }
                                              << ContactGroupList{ contactGroup2CreateData }
                                              << ContactGroupList{ contactGroup2Created };

        QTest::newRow("simple contactGroup3") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup3CreateRequest }
                                              << ContactGroupList{ contactGroup3CreateData }
                                              << ContactGroupList{ contactGroup3Created };

        QTest::newRow("batch create") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2CreateRequest, contactGroup3CreateRequest }
                                      << ContactGroupList{ contactGroup2CreateData, contactGroup3CreateData }
                                      << ContactGroupList{ contactGroup2Created, contactGroup3Created };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactGroupList, contactGroupCreateData);
        QFETCH(ContactGroupList, contactGroupCreated);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new ContactGroupCreateJob(contactGroupCreateData, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), contactGroupCreateData.count());

        for (auto i = 0; i < contactGroupCreated.count(); ++i) {
            const auto returnedContactGroup = items.at(i).dynamicCast<ContactGroup>();
            QVERIFY(returnedContactGroup);
            QCOMPARE(*returnedContactGroup, *contactGroupCreated.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::ContactGroupCreateJobTest)

#include "contactgroupcreatejobtest.moc"
