/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QSignalSpy>
#include <QTest>

#include "contactstestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "contact.h"
#include "contactfetchphotojob.h"
#include "types.h"

#include <iostream>

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::ContactsList)

class ContactFetchPhotoJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testFetch_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<ContactsList>("contacts");
        QTest::addColumn<QVector<QByteArray>>("images");

        auto contact2 = contactFromFile(QFINDTESTDATA("data/contact2.xml"));
        KContacts::Picture picture;
        picture.setRawData(
            QByteArray::fromBase64("iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg=="),
            QStringLiteral("png"));
        contact2->setPhoto(picture);
        QTest::addRow("fetch all")
            << QList<FakeNetworkAccessManager::Scenario>{scenarioFromFile(QFINDTESTDATA("data/contact1_fetch_photo_request.txt"),
                                                                          QFINDTESTDATA("data/contact1_fetch_photo_response.txt")),
                                                         scenarioFromFile(QFINDTESTDATA("data/contact2_fetch_photo_request.txt"),
                                                                          QFINDTESTDATA("data/contact2_fetch_photo_response.txt"))}
            << ContactsList{contactFromFile(QFINDTESTDATA("data/contact1.xml")), contactFromFile(QFINDTESTDATA("data/contact2.xml"))}
            << QVector<QByteArray>{{},
                                   QByteArray::fromBase64(
                                       "iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==")};
    }

    void testFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsList, contacts);
        QFETCH(QVector<QByteArray>, images);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactFetchPhotoJob(contacts, account);
        QSignalSpy fetchSpy(job, &ContactFetchPhotoJob::photoFetched);
        QVERIFY(execJob(job));
        QCOMPARE(fetchSpy.count(), 1);

        for (int i = 0; i < contacts.count(); ++i) {
            const auto contact = contacts.at(i);
            const auto image = images.at(i);
            if (image.isNull()) {
                QVERIFY(contact->photo().isEmpty());
            } else {
                QCOMPARE(contact->photo().rawData(), image);
            }
        }
    }
};

QTEST_GUILESS_MAIN(ContactFetchPhotoJobTest)

#include "contactfetchphotojobtest.moc"
