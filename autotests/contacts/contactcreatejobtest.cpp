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
#include "contactcreatejob.h"
#include "contact.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsList)

class ContactCreateJobTest : public QObject
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
        QTest::addColumn<ContactsList>("contacts");

        QTest::newRow("simple contact")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_create_request.txt"),
                                     QFINDTESTDATA("data/contact1_create_response.txt"))
                }
            << ContactsList{ contactFromFile(QFINDTESTDATA("data/contact1.xml")) };

        auto contact2 = contactFromFile(QFINDTESTDATA("data/contact2.xml"));
        KContacts::Picture picture;
        picture.setRawData(QByteArray::fromBase64("iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="), QStringLiteral("png"));
        contact2->setPhoto(picture);
        QTest::newRow("with photo")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_photo_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_photo_response.txt"))
                }
            << ContactsList{ contact2 };

        QTest::newRow("batch create")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_create_request.txt"),
                                     QFINDTESTDATA("data/contact1_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/contact2_create_photo_request.txt"),
                                     QFINDTESTDATA("data/contact2_create_photo_response.txt"))
                }
            << ContactsList{
                    contactFromFile(QFINDTESTDATA("data/contact1.xml")),
                    contact2
                };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsList, contacts);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactCreateJob(contacts, account);
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

QTEST_GUILESS_MAIN(ContactCreateJobTest)

#include "contactcreatejobtest.moc"

