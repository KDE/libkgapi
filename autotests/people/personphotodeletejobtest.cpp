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
#include "people/personphotodeletejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonPhotoDeleteJobTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        NetworkAccessManagerFactory::setFactory(new FakeNetworkAccessManagerFactory);
    }

    void testPhotoDelete_data()
    {
        QTest::addColumn<QList<FakeNetworkAccessManager::Scenario>>("scenarios");
        QTest::addColumn<QString>("peopleResourceName");
        QTest::addColumn<PersonList>("peopleModified");

        const auto person1 = TestUtils::personFromFile(QFINDTESTDATA("data/person1.json"));
        const auto person2 = TestUtils::personFromFile(QFINDTESTDATA("data/person2.json"));
        const auto person1DeletePhotoRequest = scenarioFromFile(QFINDTESTDATA("data/person1_photo_delete_request.txt"), QFINDTESTDATA("data/person1_photo_delete_response.txt"));
        const auto person2DeletePhotoRequest = scenarioFromFile(QFINDTESTDATA("data/person2_photo_delete_request.txt"), QFINDTESTDATA("data/person2_photo_delete_response.txt"));
        const auto person1DeletedPhoto = TestUtils::personFromPhotoUpdateResponseFile(QFINDTESTDATA("data/person1_photo_delete_finished_data.json"));
        const auto person2DeletedPhoto = TestUtils::personFromPhotoUpdateResponseFile(QFINDTESTDATA("data/person2_photo_delete_finished_data.json"));

        QTest::newRow("simple person1") << QList<FakeNetworkAccessManager::Scenario>{ person1DeletePhotoRequest }
                                        << person1->resourceName()
                                        << People::PersonList{ person1DeletedPhoto };

        QTest::newRow("simple person2") << QList<FakeNetworkAccessManager::Scenario>{ person2DeletePhotoRequest }
                                        << person2->resourceName()
                                        << People::PersonList{ person2DeletedPhoto };
    }

    void testPhotoDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QString, peopleResourceName);
        QFETCH(PersonList, peopleModified);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new PersonPhotoDeleteJob(peopleResourceName, account);
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

QTEST_GUILESS_MAIN(KGAPI2::People::PersonPhotoDeleteJobTest)

#include "personphotodeletejobtest.moc"
