/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
        QTest::addColumn<bool>("uidOnly");

        QTest::newRow("simple calendar")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar1_delete_response.txt"))
                }
            << CalendarsList{ calendarFromFile(QFINDTESTDATA("data/calendar1.json")) }
            << false;

        QTest::newRow("simple calendar (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar1_delete_response.txt"))
                }
            << CalendarsList{ calendarFromFile(QFINDTESTDATA("data/calendar1.json")) }
            << true;

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
                }
            << false;

        QTest::newRow("batch delete (uid)")
            << QList<FakeNetworkAccessManager::Scenario>{
                    scenarioFromFile(QFINDTESTDATA("data/calendar1_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar1_delete_response.txt")),
                    scenarioFromFile(QFINDTESTDATA("data/calendar2_delete_request.txt"),
                                     QFINDTESTDATA("data/calendar2_delete_response.txt"))
                }
            << CalendarsList{
                    calendarFromFile(QFINDTESTDATA("data/calendar1.json")),
                    calendarFromFile(QFINDTESTDATA("data/calendar2.json"))
                }
            << true;
    }

    void testDelete()
    {
        QFETCH(QList<FakeNetworkAccessManager::Scenario>, scenarios);
        QFETCH(CalendarsList, calendars);
        QFETCH(bool, uidOnly);

        FakeNetworkAccessManagerFactory::get()->setScenarios(scenarios);

        auto account = AccountPtr::create(QStringLiteral("MockAccount"), QStringLiteral("MockToken"));
        CalendarDeleteJob *job = nullptr;
        if (calendars.count() == 1) {
            if (uidOnly) {
                job = new CalendarDeleteJob(calendars.at(0)->uid(), account, nullptr);
            } else {
                job = new CalendarDeleteJob(calendars.at(0), account, nullptr);
            }
        } else {
            if (uidOnly) {
                job = new CalendarDeleteJob(elementsToUids(calendars), account, nullptr);
            } else {
                job = new CalendarDeleteJob(calendars, account, nullptr);
            }
        }
        QVERIFY(execJob(job));
    }
};

QTEST_GUILESS_MAIN(CalendarDeleteJobTest)

#include "calendardeletejobtest.moc"


