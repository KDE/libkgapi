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
#include "calendartestutils.h"

#include "types.h"
#include "calendardeletejob.h"
#include "calendar.h"
#include "account.h"

using namespace KGAPI2;

Q_DECLARE_METATYPE(QList<FakeNetworkAccessManager::Scenario>)
Q_DECLARE_METATYPE(KGAPI2::CalendarsList)

class CalendarDeleteJobTest : public QObject
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
        QTest::addColumn<CalendarsList>("calendars");

        QTest::newRow("simple calendar")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar1_delete_response.txt"))
                }
            << CalendarsList{ calendarFromFile(QFINDTESTDATA("data/calendar1.json")) };

        QTest::newRow("batch delete")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/calendar2_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar2_delete_response.txt"))
                }
            << CalendarsList{
                    calendarFromFile(QFINDTESTDATA("data/calendar1.json")),
                    calendarFromFile(QFINDTESTDATA("data/calendar2.json"))
                };
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(CalendarsList, calendars);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        auto job = new CalendarDeleteJob(calendars, account, nullptr);
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(CalendarDeleteJobTest)

#include "calendardeletejobtest.moc"


