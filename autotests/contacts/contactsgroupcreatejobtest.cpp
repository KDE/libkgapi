/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "contactstestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "contactsgroup.h"
#include "contactsgroupcreatejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsGroupsList)

class ContactsGroupCreateJobTest : public QObject
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
        QTest::addColumn<ContactsGroupsList>("groups");

        QTest::newRow("simple group") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contacts_group1_create_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group1_create_response.txt"))}
                                      << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml"))};

        auto contactsGroup2 = contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"));
        contactsGroup2->setIsSystemGroup(false); // you can't create a system group
        QTest::newRow("batch create") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contacts_group1_create_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group1_create_response.txt")),
                                                                                   scenarioFromFile(QFINDTESTDATA("data/contacts_group2_create_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group2_create_response.txt"))}
                                      << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")), contactsGroup2};
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsGroupsList, groups);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactsGroupCreateJob *job = nullptr;
        if (groups.count() == 1) {
            job = new ContactsGroupCreateJob(groups.at(0), account);
        } else {
            job = new ContactsGroupCreateJob(groups, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), groups.count());
        for (int i = 0; i < groups.count(); ++i) {
            const auto returnedGroup = items.at(i).dynamicCast<ContactsGroup>();
            QVERIFY(returnedGroup);
            QCOMPARE(*returnedGroup, *groups.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(ContactsGroupCreateJobTest)

#include "contactsgroupcreatejobtest.moc"
