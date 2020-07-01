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
#include "contactmodifyjob.h"
#include "contact.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsList)

class ContactModifyJobTest : public QObject
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
        QTest::addColumn<ContactsList>("contacts");

        QTest::newRow("change contact without photo")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_modify_request.txt"),
                                     QFINDTESTDATA("data/contact1_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_photo_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_photo_response.txt"))
                }
            << ContactsList{ contactFromFile(QFINDTESTDATA("data/contact1.xml")) };

        auto contact2 = contactFromFile(QFINDTESTDATA("data/contact2.xml"));
        KContacts::Picture picture;
        picture.setRawData(QByteArray::fromBase64("iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="), QStringLiteral("png"));
        contact2->setPhotoUrl(QStringLiteral("https://www.google.com/m8/feeds/photos/media/MockAccount/2d71e4bb897f47a8"));
        contact2->setPhoto(picture);
        QTest::newRow("change contact with photo")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact2_modify_request.txt"),
                                     QFINDTESTDATA("data/contact2_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_photo_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_photo_response.txt"))
                }
            << ContactsList{ contact2 };

        QTest::newRow("batch modify")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_modify_request.txt"),
                                     QFINDTESTDATA("data/contact1_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_photo_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_photo_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_modify_request.txt"),
                                     QFINDTESTDATA("data/contact2_modify_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_photo_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_photo_response.txt"))
                }
            << ContactsList{
                    contactFromFile(QFINDTESTDATA("data/contact1.xml")),
                    contact2
                };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsList, contacts);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        ContactModifyJob *job = nullptr;
        if (contacts.count() == 1) {
            job = new ContactModifyJob(contacts.at(0), account);
        } else {
            job = new ContactModifyJob(contacts, account);
        }
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), contacts.count());
        for (int i = 0; i < contacts.count(); ++i) {
            const auto returnedContact =  items.at(i).dynamicCast<Contact>();
            QVERIFY(returnedContact);
            QCOMPARE(*returnedContact, *contacts.at(i));
        }
    }
};

QTEST_GUILESS_MAIN(ContactModifyJobTest)

#include "contactmodifyjobtest.moc"

