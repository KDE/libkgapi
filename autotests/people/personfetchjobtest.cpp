/*
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QObject>
#include <QTest>

#include <KContacts/Picture>

#include "peopleservice.h"
#include "peopletestutils.h"
#include "fakenetworkaccessmanagerfactory.h"
#include "testutils.h"

#include "account.h"
#include "types.h"
#include "people/person.h"
#include "people/personfetchjob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonFetchJobTest : public QObject
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
        QTest::addColumn<QString>("personResourceName");
        QTest::addColumn<People::PersonList>("peopleFetched");

        const auto person1FetchRequest = scenarioFromFile(QFINDTESTDATA("data/person1_fetch_request.txt"), QFINDTESTDATA("data/person1_fetch_response.txt"));
        const auto person2FetchRequest = scenarioFromFile(QFINDTESTDATA("data/person2_fetch_request.txt"), QFINDTESTDATA("data/person2_fetch_response.txt"));
        const auto fullConnectionsFetchRequest = scenarioFromFile(QFINDTESTDATA("data/connections_initial_fetch_request.txt"), QFINDTESTDATA("data/connections_full_fetch_response.txt"));
        const auto partialConnectionsInitialFetchRequest = scenarioFromFile(QFINDTESTDATA("data/connections_initial_fetch_request.txt"), QFINDTESTDATA("data/connections_partial_fetch_response_1.txt"));
        const auto partialConnectionsFollowupFetchRequest = scenarioFromFile(QFINDTESTDATA("data/connections_partial_followup_fetch_request.txt"), QFINDTESTDATA("data/connections_partial_fetch_response_2.txt"));
        const auto person1 = TestUtils::personFromFile(QFINDTESTDATA("data/person1.json"));
        const auto person2 = TestUtils::personFromFile(QFINDTESTDATA("data/person2.json"));

        QTest::newRow("simple person1 fetch") << QList<FakeNetworkAccessManager::Scenario>{ person1FetchRequest }
                                              << person1->resourceName()
                                              << People::PersonList{ person1 };

        QTest::newRow("simple person2 fetch") << QList<FakeNetworkAccessManager::Scenario>{ person2FetchRequest }
                                              << person2->resourceName()
                                              << People::PersonList{ person2 };

        QTest::newRow("full connections fetch") << QList<FakeNetworkAccessManager::Scenario>{ fullConnectionsFetchRequest }
                                                << QString()
                                                << People::PersonList{ person1, person2 };

        QTest::newRow("partial connections fetch") << QList<FakeNetworkAccessManager::Scenario>{ partialConnectionsInitialFetchRequest, partialConnectionsFollowupFetchRequest }
                                                   << QString()
                                                   << People::PersonList{ person1, person2 };
    }

    void testFetch()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(QString, personResourceName);
        QFETCH(PersonList, peopleFetched);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new PersonFetchJob(personResourceName, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), peopleFetched.count());

        for (auto i = 0; i < peopleFetched.count(); ++i) {
            const auto returnedPerson = items.at(i).dynamicCast<Person>();
            QVERIFY(returnedPerson);
            QCOMPARE(*returnedPerson, *peopleFetched.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::PersonFetchJobTest)

#include "personfetchjobtest.moc"
