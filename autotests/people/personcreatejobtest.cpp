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
#include "people/personcreatejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonCreateJobTest : public QObject
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
        QTest::addColumn<People::PersonList>("peopleCreateData");
        QTest::addColumn<People::PersonList>("peopleCreated");

        const auto person1CreateData = TestUtils::personFromFile(QFINDTESTDATA("data/person1_create_data.json"));
        const auto person2CreateData = TestUtils::personFromFile(QFINDTESTDATA("data/person2_create_data.json"));
        const auto person1CreateRequest = scenarioFromFile(QFINDTESTDATA("data/person1_create_request.txt"), QFINDTESTDATA("data/person1_create_response.txt"));
        const auto person2CreateRequest = scenarioFromFile(QFINDTESTDATA("data/person2_create_request.txt"), QFINDTESTDATA("data/person2_create_response.txt"));
        const auto person1Created = TestUtils::personFromFile(QFINDTESTDATA("data/person1.json"));
        const auto person2Created = TestUtils::personFromFile(QFINDTESTDATA("data/person2.json"));
        QTest::newRow("simple person1") << QList<FakeNetworkAccessManager::Scenario>{ person1CreateRequest }
                                        << People::PersonList{ person1CreateData }
                                        << People::PersonList{ person1Created };

        QTest::newRow("simple person2") << QList<FakeNetworkAccessManager::Scenario>{ person2CreateRequest }
                                        << People::PersonList{ person2CreateData }
                                        << People::PersonList{ person2Created };

        QTest::newRow("batch create") << QList<FakeNetworkAccessManager::Scenario>{ person1CreateRequest, person2CreateRequest }
                                      << PersonList{ person1CreateData, person2CreateData }
                                      << People::PersonList{ person1Created, person2Created };
    }

    void testCreate()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(PersonList, peopleCreateData);
        QFETCH(PersonList, peopleCreated);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new PersonCreateJob(peopleCreateData, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), peopleCreateData.count());

        for (auto i = 0; i < peopleCreated.count(); ++i) {
            const auto returnedPerson = items.at(i).dynamicCast<Person>();
            QVERIFY(returnedPerson);
            QCOMPARE(*returnedPerson, *peopleCreated.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::PersonCreateJobTest)

#include "personcreatejobtest.moc"
