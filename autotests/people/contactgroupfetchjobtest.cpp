/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include <KContacts/Picture>

#include "peopleservice.h"
#include "peopletestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "types.h"
#include "people/contactgroup.h"
#include "people/contactgroupfetchjob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::ContactGroupList)

namespace KGAPI2 {
namespace People {

class ContactGroupFetchJobTest : public QObject
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
        QTest::addColumn<QString>("contactGroupsResourceName");
        QTest::addColumn<People::ContactGroupList>("contactGroupsFetched");

        const auto contactGroup1FetchRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup1_fetch_request.txt"), QFINDTESTDATA("data/contactgroup1_fetch_response.txt"));
        const auto contactGroup2FetchRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup2_fetch_request.txt"), QFINDTESTDATA("data/contactgroup2_fetch_response.txt"));
        const auto fullContactGroupListFetchRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup_list_fetch_request.txt"), QFINDTESTDATA("data/contactgroup_list_fetch_full_response.txt"));
        const auto partialContactGroupListInitialFetchRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup_list_fetch_request.txt"), QFINDTESTDATA("data/contactgroup_list_fetch_partial_response_1.txt"));
        const auto partialContactGroupListFollowupFetchRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup_list_fetch_followup_request.txt"), QFINDTESTDATA("data/contactgroup_list_fetch_partial_response_2.txt"));
        const auto contactGroup1 = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup1.json"));
        const auto contactGroup2 = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup2.json"));

        QTest::newRow("simple contactGroup1 fetch") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup1FetchRequest }
                                                    << contactGroup1->resourceName()
                                                    << People::ContactGroupList{ contactGroup1 };

        QTest::newRow("simple contactGroup2 fetch") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2FetchRequest }
                                                    << contactGroup2->resourceName()
                                                    << People::ContactGroupList{ contactGroup2 };

        QTest::newRow("full contactGroups fetch") << QList<FakeNetworkAccessManager::Scenario>{ fullContactGroupListFetchRequest }
                                                  << QString()
                                                  << People::ContactGroupList{ contactGroup1, contactGroup2 };

        QTest::newRow("partial contactGroups fetch") << QList<FakeNetworkAccessManager::Scenario>{ partialContactGroupListInitialFetchRequest, partialContactGroupListFollowupFetchRequest }
                                                     << QString()
                                                     << People::ContactGroupList{ contactGroup1, contactGroup2 };
    }

    void testFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QString, contactGroupsResourceName);
        QFETCH(ContactGroupList, contactGroupsFetched);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new ContactGroupFetchJob(contactGroupsResourceName, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), contactGroupsFetched.count());

        for (auto i = 0; i < contactGroupsFetched.count(); ++i) {
            const auto returnedContactGroup = items.at(i).dynamicCast<ContactGroup>();
            QVERIFY(returnedContactGroup);
            QCOMPARE(*returnedContactGroup, *contactGroupsFetched.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::ContactGroupFetchJobTest)

#include "contactgroupfetchjobtest.moc"
