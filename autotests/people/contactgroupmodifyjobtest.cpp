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
#include "people/contactgroupmodifyjob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::ContactGroupList)

namespace KGAPI2 {
namespace People {

class ContactGroupModifyJobTest : public QObject
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
        QTest::addColumn<People::ContactGroupList>("peopleModifyData");
        QTest::addColumn<People::ContactGroupList>("peopleModified");

        const auto contactGroup2ModifyData = TestUtils::contactGroupFromRequestDataFile(QFINDTESTDATA("data/contactgroup2_modify_data.json"));
        const auto contactGroup3ModifyData = TestUtils::contactGroupFromRequestDataFile(QFINDTESTDATA("data/contactgroup3_modify_data.json"));
        const auto contactGroup2ModifyRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup2_modify_request.txt"), QFINDTESTDATA("data/contactgroup2_modify_response.txt"));
        const auto contactGroup3ModifyRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup3_modify_request.txt"), QFINDTESTDATA("data/contactgroup3_modify_response.txt"));
        const auto contactGroup2Modified = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup2_modify_finished_data.json"));
        const auto contactGroup3Modified = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup3_modify_finished_data.json"));
        QTest::newRow("simple contactgroup2") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2ModifyRequest }
                                              << People::ContactGroupList{ contactGroup2ModifyData }
                                              << People::ContactGroupList{ contactGroup2Modified };

        QTest::newRow("simple contactgroup3") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup3ModifyRequest }
                                              << People::ContactGroupList{ contactGroup3ModifyData }
                                              << People::ContactGroupList{ contactGroup3Modified };

        QTest::newRow("batch modify") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2ModifyRequest, contactGroup3ModifyRequest }
                                      << ContactGroupList{ contactGroup2ModifyData, contactGroup3ModifyData }
                                      << People::ContactGroupList{ contactGroup2Modified, contactGroup3Modified };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactGroupList, peopleModifyData);
        QFETCH(ContactGroupList, peopleModified);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new ContactGroupModifyJob(peopleModifyData, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), peopleModifyData.count());

        for (auto i = 0; i < peopleModified.count(); ++i) {
            const auto returnedContactGroup = items.at(i).dynamicCast<ContactGroup>();
            QVERIFY(returnedContactGroup);
            QCOMPARE(*returnedContactGroup, *peopleModified.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::ContactGroupModifyJobTest)

#include "contactgroupmodifyjobtest.moc"
