/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "contactstestutils.h"

#include "types.h"
#include "contactsgroupfetchjob.h"
#include "contactsgroup.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsGroupPtr)

class ContactsGroupFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetchAll()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/contacts_groups_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/contacts_groups_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/contacts_groups_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/contacts_groups_fetch_page2_response.txt"))
        });
        const ContactsGroupsList groups = {
            contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")),
            contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactsGroupFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), groups.count());
        for (int i = 0; i < groups.count(); ++i) {
            const auto returnedGroup =  items.at(i).dynamicCast<ContactsGroup>();
            QVERIFY(returnedGroup);
            QCOMPARE(*returnedGroup, *groups.at(i));
        }
    }

    void testFetchSingle_data()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/contacts_group2_fetch_request.txt"),
                             QFINDTESTDATA("data/contacts_group1_fetch_response.txt"))
        });
        const auto group = contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactsGroupFetchJob(group->id(), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedGroup =  items.at(0).dynamicCast<ContactsGroup>();
        QVERIFY(returnedGroup);
        QCOMPARE(*returnedGroup, *group);
    }
};

QTEST_GUILESS_MAIN(ContactsGroupFetchJobTest)

#include "contactsgroupfetchjobtest.moc"
