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
#include "contactdeletejob.h"
#include "contact.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsList)

class ContactDeleteJobTest : public QObject
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
        QTest::addColumn<ContactsList>("contacts");
        QTest::addColumn<bool>("uidOnly");

        QTest::newRow("simple contact")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_response.txt"))
                }
            << ContactsList{ contactFromFile(QFINDTESTDATA("data/contact1.xml")) }
            << false;

        QTest::newRow("simple contact (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_response.txt"))
                }
            << ContactsList{ contactFromFile(QFINDTESTDATA("data/contact1.xml")) }
            << true;

        QTest::newRow("batch delete")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_delete_request.txt"),
                                     QFINDTESTDATA("data/contact2_delete_response.txt"))
                }
            << ContactsList{
                    contactFromFile(QFINDTESTDATA("data/contact1.xml")),
                    contactFromFile(QFINDTESTDATA("data/contact2.xml"))
                }
            << false;


        QTest::newRow("batch delete (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_delete_request.txt"),
                                     QFINDTESTDATA("data/contact2_delete_response.txt"))
                }
            << ContactsList{
                    contactFromFile(QFINDTESTDATA("data/contact1.xml")),
                    contactFromFile(QFINDTESTDATA("data/contact2.xml"))
                }
            << true;

    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsList, contacts);
        QFETCH(bool, uidOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactDeleteJob *job = nullptr;
        if (contacts.count() == 1) {
            if (uidOnly) {
                job = new ContactDeleteJob(contacts.at(0)->uid(), account, nullptr);
            } else {
                job = new ContactDeleteJob(contacts.at(0), account, nullptr);
            }
        } else {
            if (uidOnly) {
                job = new ContactDeleteJob(elementsToUids(contacts), account, nullptr);
            } else {
                job = new ContactDeleteJob(contacts, account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(ContactDeleteJobTest)

#include "contactdeletejobtest.moc"

