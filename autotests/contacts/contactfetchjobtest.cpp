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
#include "contactfetchjob.h"
#include "contact.h"
#include "account.h"

#include <iostream>

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsList)

class ContactFetchJobTest : public QObject
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
            scenarioFromFile(QFINDTESTDATA("data/contacts_fetch_page1_request.txt"),
                             QFINDTESTDATA("data/contacts_fetch_page1_response.txt")),
            scenarioFromFile(QFINDTESTDATA("data/contacts_fetch_page2_request.txt"),
                             QFINDTESTDATA("data/contacts_fetch_page2_response.txt"))
        });
        const ContactsList contacts = {
            contactFromFile(QFINDTESTDATA("data/contact1.xml")),
            contactFromFile(QFINDTESTDATA("data/contact2.xml"))
        };

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactFetchJob(account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), contacts.count());
        for (int i = 0; i < contacts.count(); ++i) {
            const auto returnedContact =  items.at(i).dynamicCast<Contact>();
            const auto expectedContact = contacts.at(i);
            QVERIFY(returnedContact);
            QCOMPARE(*returnedContact, *expectedContact);
        }
    }

    void testFetchSingle()
    {
        FakeNetworkAccessManagerFactory::get()->setScenarios({
            scenarioFromFile(QFINDTESTDATA("data/contact1_fetch_request.txt"),
                             QFINDTESTDATA("data/contact1_fetch_response.txt"))

        });
        const auto contact = contactFromFile(QFINDTESTDATA("data/contact1.xml"));

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactFetchJob(contact->uid(), account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), 1);
        const auto returnedContact =  items.at(0).dynamicCast<Contact>();
        QVERIFY(returnedContact);
        QCOMPARE(*returnedContact, *contact);
    }
};

QTEST_GUILESS_MAIN(ContactFetchJobTest)

#include "contactfetchjobtest.moc"


