/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QTest>

#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"
#include "contactstestutils.h"

#include "types.h"
#include "contactsgroupmodifyjob.h"
#include "contactsgroup.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsGroupsList)

class ContactsGroupModifyJobTest : public QObject
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
        QTest::addColumn<ContactsGroupsList>("groups");

        QTest::newRow("simple group")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contacts_group1_modify_request.txt"),
                                     QFINDTESTDATA("data/contacts_group1_modify_response.txt"))
                }
            << ContactsGroupsList{ contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")) };

        auto contactsGroup2 = contactsGroupFromFile(QFINDTESTDATA("data/contacts_group2.xml"));
        contactsGroup2->setIsSystemGroup(false); // you can't modify a system group
        QTest::newRow("batch modify")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contacts_group1_modify_request.txt"),
                                     QFINDTESTDATA("data/contacts_group1_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contacts_group2_modify_request.txt"),
                                     QFINDTESTDATA("data/contacts_group2_modify_response.txt"))
                }
            << ContactsGroupsList{
                    contactsGroupFromFile(QFINDTESTDATA("data/contacts_group1.xml")),
                    contactsGroup2
                };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsGroupsList, groups);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactsGroupModifyJob *job = nullptr;
        if (groups.count() == 1) {
            job = new ContactsGroupModifyJob(groups.at(0), account);
        } else {
            job = new ContactsGroupModifyJob(groups, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), groups.count());
        for (int i = 0; i < groups.count(); ++i) {
            const auto returnedGroup =  items.at(i).dynamicCast<ContactsGroup>();
            QVERIFY(returnedGroup);
            QCOMPARE(*returnedGroup, *groups.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(ContactsGroupModifyJobTest)

#include "contactsgroupmodifyjobtest.moc"

