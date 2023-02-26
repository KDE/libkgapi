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
#include "people/personmodifyjob.h"

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::People::PersonList)

namespace KGAPI2 {
namespace People {

class PersonModifyJobTest : public QObject
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
        QTest::addColumn<People::PersonList>("peopleModifyData");
        QTest::addColumn<People::PersonList>("peopleModified");

        const auto person1ModifyData = TestUtils::personFromFile(QFINDTESTDATA("data/person1_modify_data.json"));
        const auto person2ModifyData = TestUtils::personFromFile(QFINDTESTDATA("data/person2_modify_data.json"));
        const auto person1ModifyRequest = scenarioFromFile(QFINDTESTDATA("data/person1_modify_request.txt"), QFINDTESTDATA("data/person1_modify_response.txt"));
        const auto person2ModifyRequest = scenarioFromFile(QFINDTESTDATA("data/person2_modify_request.txt"), QFINDTESTDATA("data/person2_modify_response.txt"));
        const auto person1Modified = TestUtils::personFromFile(QFINDTESTDATA("data/person1_modify_finished_data.json"));
        const auto person2Modified = TestUtils::personFromFile(QFINDTESTDATA("data/person2_modify_finished_data.json"));
        QTest::newRow("simple person1") << QList<FakeNetworkAccessManager::Scenario>{ person1ModifyRequest }
                                        << People::PersonList{ person1ModifyData }
                                        << People::PersonList{ person1Modified };

        QTest::newRow("simple person2") << QList<FakeNetworkAccessManager::Scenario>{ person2ModifyRequest }
                                        << People::PersonList{ person2ModifyData }
                                        << People::PersonList{ person2Modified };

        QTest::newRow("batch modify") << QList<FakeNetworkAccessManager::Scenario>{ person1ModifyRequest, person2ModifyRequest }
                                      << PersonList{ person1ModifyData, person2ModifyData }
                                      << People::PersonList{ person1Modified, person2Modified };
    }

    void testModify()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(PersonList, peopleModifyData);
        QFETCH(PersonList, peopleModified);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        const auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        const auto job = new PersonModifyJob(peopleModifyData, account);
        QVERIFY(execJob(job));
        const auto items = job->items();
        QCOMPARE(items.count(), peopleModifyData.count());

        for (auto i = 0; i < peopleModified.count(); ++i) {
            const auto returnedPerson = items.at(i).dynamicCast<Person>();
            QVERIFY(returnedPerson);
            QCOMPARE(*returnedPerson, *peopleModified.at(i));
        }
    }
};

}
}

QTEST_GUILESS_MAIN(KGAPI2::People::PersonModifyJobTest)

#include "personmodifyjobtest.moc"
