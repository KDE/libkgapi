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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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
