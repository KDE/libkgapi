/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "objectscalendartests.h"

#include <kcalcore/event.h>

#include <objects/calendar.h>

using namespace KGAPI;
using namespace KCalCore;

Q_DECLARE_METATYPE(KCalCore::Alarm::Type);
Q_DECLARE_METATYPE(KCalCore::Incidence::Ptr);
Q_DECLARE_METATYPE(KGAPI::Objects::Reminder::List);

void ObjectsCalendarTests::testReminder_data()
{
    QTest::addColumn< Alarm::Type >("type");
    QTest::addColumn< Duration >("startOffset");
    QTest::addColumn< Alarm::Ptr >("alarm");
    QTest::addColumn< Incidence::Ptr >("incidence");

    Incidence::Ptr incidence(new Event());
    incidence->setDtStart(QDateTime::fromString("2012-01-01T00:00:00.000Z", Qt::ISODate));
    incidence->setDuration(Duration(1, KCalCore::Duration::Days));

    Alarm::Ptr alarm(new Alarm(incidence.data()));
    alarm->setType(KCalCore::Alarm::Display);
    alarm->setStartOffset(Duration(10, Duration::Seconds));

    QTest::newRow("reminder1")
            << Alarm::Display
            << Duration (10, Duration::Seconds)
            << alarm
            << incidence;
}

void ObjectsCalendarTests::testReminder()
{
    KGAPI::Objects::Reminder reminder;
    KCalCore::Alarm::Ptr oAlarm;

    QFETCH(Alarm::Type, type);
    QFETCH(Duration, startOffset);
    QFETCH(Alarm::Ptr, alarm);
    QFETCH(Incidence::Ptr, incidence);

    reminder.setType(type);
    reminder.setStartOffset(startOffset);

    QCOMPARE(reminder.type(), type);
    QCOMPARE(reminder.startOffset(), startOffset);

    oAlarm = reminder.toAlarm(incidence.data());
    QCOMPARE(oAlarm->type(), alarm->type());
    QCOMPARE(oAlarm->startOffset(), alarm->startOffset());
}

void ObjectsCalendarTests::testCalendar_data()
{
    QTest::addColumn< QString >("uid");
    QTest::addColumn< QString >("title");
    QTest::addColumn< QString >("details");
    QTest::addColumn< QString >("location");
    QTest::addColumn< QString >("timezone");
    QTest::addColumn< bool >("editable");
    QTest::addColumn< Objects::Reminder::List >("reminders");

    Objects::Reminder::List reminders;
    Objects::Reminder::Ptr reminder(new Objects::Reminder);
    reminder->setType(KCalCore::Alarm::Email);
    reminder->setStartOffset(Duration(1000, Duration::Seconds));
    reminders << reminder;

    QTest::newRow("calendar1_cz")
            << "1234abcde"
            << "Můj Kalendář"
            << "Nějaké detaily k mému kalendáři"
            << "Czech Republic"
            << "Prague/Europe"
            << true
            << reminders;
}

void ObjectsCalendarTests::testCalendar()
{
    Objects::Calendar calendar;

    QFETCH(QString, uid);
    QFETCH(QString, title);
    QFETCH(QString, details);
    QFETCH(QString, location);
    QFETCH(QString, timezone);
    QFETCH(bool, editable);
    QFETCH(Objects::Reminder::List, reminders);

    calendar.setUid(uid);
    calendar.setTitle(title);
    calendar.setDetails(details);
    calendar.setLocation(location);
    calendar.setTimezone(timezone);
    calendar.setEditable(editable);
    calendar.setDefaultReminders(reminders);

    QCOMPARE(calendar.uid(), uid);
    QCOMPARE(calendar.title(), title);
    QCOMPARE(calendar.details(), details);
    QCOMPARE(calendar.location(), location);
    QCOMPARE(calendar.timezone(), timezone);
    QCOMPARE(calendar.editable(), editable);
    QCOMPARE(calendar.defaultReminders(), reminders);
}

QTEST_KDEMAIN(ObjectsCalendarTests, NoGUI);
