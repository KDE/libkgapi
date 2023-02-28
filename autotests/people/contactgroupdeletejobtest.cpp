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
#include "people/contactgroupdeletejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::ContactGroupList)

namespace KGAPI2 {
namespace People {

class ContactGroupDeleteJobTest : public QObject
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
        QTest::addColumn<ContactGroupList>("peopleToDelete");

        const auto contactGroup2DeleteRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup2_delete_request.txt"), QFINDTESTDATA("data/contactgroup2_delete_response.txt"));
        const auto contactGroup3DeleteRequest = scenarioFromFile(QFINDTESTDATA("data/contactgroup3_delete_request.txt"), QFINDTESTDATA("data/contactgroup3_delete_response.txt"));
        const auto contactGroup2 = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup2.json"));
        const auto contactGroup3 = TestUtils::contactGroupFromFile(QFINDTESTDATA("data/contactgroup3.json"));

        QTest::newRow("contactGroup2 resourceName delete") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2DeleteRequest }
                                                     << ContactGroupList{ contactGroup2 };

        QTest::newRow("contactGroup3 resourceName delete") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup3DeleteRequest }
                                                     << ContactGroupList{ contactGroup3 };

        QTest::newRow("batch resourceName delete") << QList<FakeNetworkAccessManager::Scenario>{ contactGroup2DeleteRequest, contactGroup3DeleteRequest }
                                                   << ContactGroupList{ contactGroup2, contactGroup3 };
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactGroupList, peopleToDelete);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactGroupDeleteJob *job;

        QStringList peopleResourceNames;
        std::transform(peopleToDelete.cbegin(), peopleToDelete.cend(), std::back_inserter(peopleResourceNames), [](const ContactGroupPtr &contactGroup) {
            return contactGroup->resourceName();
        });

        if (peopleResourceNames.count() == 1) {
            job = new ContactGroupDeleteJob(peopleResourceNames.first(), account);
        } else {
            job = new ContactGroupDeleteJob(peopleResourceNames, account);
        }

        QVERIFY(execJob(job));
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::ContactGroupDeleteJobTest)

#include "contactgroupdeletejobtest.moc"
