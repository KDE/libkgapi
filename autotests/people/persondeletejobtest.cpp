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
#include "people/persondeletejob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonDeleteJobTest : public QObject
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
        QTest::addColumn<PersonList>("peopleToDelete");
        QTest::addColumn<bool>("usingResourceName");

        const auto person1DeleteRequest = scenarioFromFile(QFINDTESTDATA("data/person1_delete_request.txt"), QFINDTESTDATA("data/person1_delete_response.txt"));
        const auto person2DeleteRequest = scenarioFromFile(QFINDTESTDATA("data/person2_delete_request.txt"), QFINDTESTDATA("data/person2_delete_response.txt"));
        const auto person1 = TestUtils::personFromFile(QFINDTESTDATA("data/person1.json"));
        const auto person2 = TestUtils::personFromFile(QFINDTESTDATA("data/person2.json"));
        QTest::newRow("person1 delete") << QList<FakeNetworkAccessManager::Scenario>{ person1DeleteRequest }
                                        << PersonList{ person1 }
                                        << false;

        QTest::newRow("person2 delete") << QList<FakeNetworkAccessManager::Scenario>{ person2DeleteRequest }
                                        << PersonList{ person2 }
                                        << false;

        QTest::newRow("person1 resourceName delete") << QList<FakeNetworkAccessManager::Scenario>{ person1DeleteRequest }
                                                     << PersonList{ person1 }
                                                     << true;

        QTest::newRow("person2 resourceName delete") << QList<FakeNetworkAccessManager::Scenario>{ person2DeleteRequest }
                                                     << PersonList{ person2 }
                                                     << true;

        QTest::newRow("batch delete") << QList<FakeNetworkAccessManager::Scenario>{ person1DeleteRequest, person2DeleteRequest }
                                      << PersonList{ person1, person2 }
                                      << false;

        QTest::newRow("batch delete") << QList<FakeNetworkAccessManager::Scenario>{ person1DeleteRequest, person2DeleteRequest }
                                      << PersonList{ person1, person2 }
                                      << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(PersonList, peopleToDelete);
        QFETCH(bool, usingResourceName);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        PersonDeleteJob *job;

        if (usingResourceName) {
            QStringList peopleResourceNames;
            std::transform(peopleToDelete.cbegin(), peopleToDelete.cend(), std::back_inserter(peopleResourceNames), [](const PersonPtr &person) {
                return person->resourceName();
            });

            if (peopleResourceNames.count() == 1) {
                job = new PersonDeleteJob(peopleResourceNames.first(), account);
            } else {
                job = new PersonDeleteJob(peopleResourceNames, account);
            }
        } else if (peopleToDelete.count() == 1) {
            job = new PersonDeleteJob(peopleToDelete.first(), account);
        } else {
            job = new PersonDeleteJob(peopleToDelete, account);
        }

        QVERIFY(execJob(job));
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::PersonDeleteJobTest)

#include "persondeletejobtest.moc"
