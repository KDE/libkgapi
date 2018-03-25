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

        QTest::newRow("simple contact")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/contact1_delete_request.txt"),
                                     QFINDTESTDATA("data/contact1_delete_response.txt"))
                }
            << ContactsList{ contactFromFile(QFINDTESTDATA("data/contact1.xml")) };

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
                };
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(ContactsList, contacts);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new ContactDeleteJob(contacts, account, nullptr);
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(ContactDeleteJobTest)

#include "contactdeletejobtest.moc"

