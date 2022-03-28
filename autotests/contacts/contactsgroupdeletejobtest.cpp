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
#include "contactsgroupdeletejob.h"
#include "types.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsGroupsList)

class ContactsGroupDeleteJobTest : public QObject
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
        QTest::addColumn<ContactsGroupsList>("groups");
        QTest::addColumn<bool>("idsOnly");

        QTest::newRow("simple group") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contacts_group1_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group1_delete_response.txt"))}
                                      << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml"))} << false;

        QTest::newRow("simple group (ids)") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(
            QFINDTESTDATA("data/contacts_group1_delete_request.txt"),
            QFINDTESTDATA("data/contacts_group1_delete_response.txt"))}
                                            << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml"))} << true;

        QTest::newRow("batch delete") << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contacts_group1_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group1_delete_response.txt")),
                                                                                   scenarioFromFile(QFINDTESTDATA("data/contacts_group2_delete_request.txt"),
                                                                                                    QFINDTESTDATA("data/contacts_group2_delete_response.txt"))}
                                      << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")),
                                                            contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"))}
                                      << false;

        QTest::newRow("batch delete (ids)")
            << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contacts_group1_delete_request.txt"),
                                                                          QFINDTESTDATA("data/contacts_group1_delete_response.txt")),
                                                         scenarioFromFile(QFINDTESTDATA("data/contacts_group2_delete_request.txt"),
                                                                          QFINDTESTDATA("data/contacts_group2_delete_response.txt"))}
            << ContactsGroupsList{contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")),
                                  contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"))}
            << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsGroupsList, groups);
        QFETCH(bool, idsOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactsGroupDeleteJob *job = nullptr;
        if (groups.count() == 1) {
            if (idsOnly) {
                job = new ContactsGroupDeleteJob(groups.at(0)->id(), account, nullptr);
            } else {
                job = new ContactsGroupDeleteJob(groups.at(0), account, nullptr);
            }
        } else {
            if (idsOnly) {
                job = new ContactsGroupDeleteJob(elementsToIds(groups), account, nullptr);
            } else {
                job = new ContactsGroupDeleteJob(groups, account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(ContactsGroupDeleteJobTest)

#include "contactsgroupdeletejobtest.moc"
