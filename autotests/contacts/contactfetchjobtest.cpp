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


