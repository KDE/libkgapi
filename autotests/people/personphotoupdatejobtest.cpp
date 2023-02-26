/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include <KContacts/Picture>

#include "peopletestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "types.h"
#include "people/person.h"
#include "people/personphotoupdatejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonPhotoUpdateJobTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testPhotoUpdate_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<QString>("peopleResourceName");
        QTest::addColumn<QByteArray>("peoplePhotoUpdateData");
        QTest::addColumn<PersonList>("peopleModified");

        const auto person1 = TestUtils::personFromFile(QFINDTESTDATA("data/person1.json"));
        const auto person2 = TestUtils::personFromFile(QFINDTESTDATA("data/person2.json"));
        const auto person1PhotoUpdateData = TestUtils::rawPhotoDataFromPhotoRequestData(QFINDTESTDATA("data/person1_photo_update_data.json"));
        const auto person2PhotoUpdateData = TestUtils::rawPhotoDataFromPhotoRequestData(QFINDTESTDATA("data/person2_photo_update_data.json"));
        const auto person1UpdatePhotoRequest = scenarioFromFile(QFINDTESTDATA("data/person1_photo_update_request.txt"), QFINDTESTDATA("data/person1_photo_update_response.txt"));
        const auto person2UpdatePhotoRequest = scenarioFromFile(QFINDTESTDATA("data/person2_photo_update_request.txt"), QFINDTESTDATA("data/person2_photo_update_response.txt"));
        const auto person1UpdatedPhoto = TestUtils::personFromPhotoUpdateResponseFile(QFINDTESTDATA("data/person1_photo_update_finished_data.json"));
        const auto person2UpdatedPhoto = TestUtils::personFromPhotoUpdateResponseFile(QFINDTESTDATA("data/person2_photo_update_finished_data.json"));

        QTest::newRow("simple person1") << QList<FakeNetworkAccessManager::Scenario>{ person1UpdatePhotoRequest }
                                        << person1->resourceName()
                                        << person1PhotoUpdateData
                                        << People::PersonList{ person1UpdatedPhoto };

        QTest::newRow("simple person2") << QList<FakeNetworkAccessManager::Scenario>{ person2UpdatePhotoRequest }
                                        << person2->resourceName()
                                        << person2PhotoUpdateData
                                        << People::PersonList{ person2UpdatedPhoto };
    }

    void testPhotoUpdate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QString, peopleResourceName);
        QFETCH(QByteArray, peoplePhotoUpdateData);
        QFETCH(PersonList, peopleModified);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new PersonPhotoUpdateJob(peopleResourceName, peoplePhotoUpdateData, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), peopleModified.count());

        for (auto i = 0; i < peopleModified.count(); ++i) {
            const auto returnedPerson = items.at(i).dynamicCast<Person>();
            QVERIFY(returnedPerson);
            QCOMPARE(*returnedPerson, *peopleModified.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::PersonPhotoUpdateJobTest)

#include "personphotoupdatejobtest.moc"
