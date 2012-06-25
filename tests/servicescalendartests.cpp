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

#include "servicescalendartests.h"
#include <services/calendar.h>
#include <objects/event.h>

#include <KDateTime>
#include <KCalCore/Event>
#include <KCalCore/ICalFormat>
#include <KDebug>

typedef QMap< QByteArray, QString > ByteArrayStringMap;

Q_DECLARE_METATYPE(KCalCore::Event::Status);
Q_DECLARE_METATYPE(KCalCore::Recurrence*);
Q_DECLARE_METATYPE(KCalCore::Event::Transparency);
Q_DECLARE_METATYPE(KCalCore::Attendee::List);
Q_DECLARE_METATYPE(KCalCore::Alarm::List);
Q_DECLARE_METATYPE(ByteArrayStringMap);

void ServicesCalendarTests::testJSONParser_data()
{
    /*
     * See https://developers.google.com/google-apps/calendar/v3/reference/events#resource
     */
    QTest::addColumn< QString >("id");
    QTest::addColumn< bool >("deleted");
    QTest::addColumn< KCalCore::Event::Status >("status");
    QTest::addColumn< KDateTime >("created");
    QTest::addColumn< KDateTime >("updated");
    QTest::addColumn< QString >("summary");
    QTest::addColumn< QString >("description");
    QTest::addColumn< QString >("location");
    QTest::addColumn< KCalCore::Person::Ptr >("creator");
    QTest::addColumn< KCalCore::Person::Ptr >("organizer");
    QTest::addColumn< KDateTime >("dtStart");
    QTest::addColumn< KDateTime >("dtEnd");
    QTest::addColumn< bool >("allDay");
    QTest::addColumn< KCalCore::Recurrence* >("recurrence");
    QTest::addColumn< KCalCore::Event::Transparency >("transparency");
    QTest::addColumn< QString >("iCalUID");
    QTest::addColumn< int >("sequence");
    QTest::addColumn< KCalCore::Attendee::List >("attendees");
    QTest::addColumn< KCalCore::Alarm::List >("reminders");
    QTest::addColumn< ByteArrayStringMap >("customProperties");
    QTest::addColumn< QByteArray >("jsonData");

    QTest::newRow("test1")
            << "h@0203488aeaa000f7e787fef631d12bc0d7325ef2"
            << false
            << KCalCore::Event::StatusConfirmed
            << KDateTime::fromString("2012-06-12T20:28:34.000Z", KDateTime::RFC3339Date)
            << KDateTime::fromString("2012-06-12T20:28:34.000Z", KDateTime::RFC3339Date)
            << QString("Den vzniku samostatného československého státu")
            << QString()
            << QString()
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KDateTime::fromString("2012-10-28", KDateTime::ISODate)
            // Note the end-date correction! */
            << KDateTime::fromString("2012-10-28", KDateTime::ISODate)
            << true
            << new KCalCore::Recurrence()
            << KCalCore::Event::Opaque
            << "h@0203488aeaa000f7e787fef631d12bc0d7325ef2@google.com"
            << 1
            << KCalCore::Attendee::List()
            << KCalCore::Alarm::List()
            << ByteArrayStringMap()
            << QString("{\n"
               "    \"kind\": \"calendar#event\",\n"
               "    \"etag\": \"\\\"Nj4ZG2eInHp1GWYvR0V2n_Z2u9Q/Q05EYWlKUC1KaEVBQUFBQUFBQUFBQT09\\\"\",\n"
               "    \"id\": \"h@0203488aeaa000f7e787fef631d12bc0d7325ef2\",\n"
               "    \"status\": \"confirmed\",\n"
               "    \"htmlLink\": \"https://www.google.com/calendar/event?eid=aEAwMjAzNDg4YWVhYTAwMGY3ZTc4N2ZlZjYzMWQxMmJjMGQ3MzI1ZWYyIGNzLmN6ZWNoI2hvbGlkYXlAdg\",\n"
               "    \"created\": \"2012-06-12T20:28:34.000Z\",\n"
               "    \"updated\": \"2012-06-12T20:28:34.000Z\",\n"
               "    \"summary\": \"Den vzniku samostatného československého státu\",\n"
               "    \"creator\": {\n"
               "        \"email\": \"cs.czech#holiday@group.v.calendar.google.com\",\n"
               "        \"displayName\": \"České svátky\",\n"
               "        \"self\": true\n"
               "    },\n"
               "    \"organizer\": {\n"
               "        \"email\": \"cs.czech#holiday@group.v.calendar.google.com\",\n"
               "        \"displayName\": \"České svátky\",\n"
               "        \"self\": true\n"
               "    },\n"
               "    \"start\": {\n"
               "        \"date\": \"2012-10-28\"\n"
               "    },\n"
               "    \"end\": {\n"
               "        \"date\": \"2012-10-29\"\n"
               "    },\n"
               "    \"visibility\": \"public\",\n"
               "    \"iCalUID\": \"h@0203488aeaa000f7e787fef631d12bc0d7325ef2@google.com\",\n"
               "    \"sequence\": 1\n"
               "}").toUtf8();

    ICalFormat format;
    KCalCore::Recurrence *recurrence = new KCalCore::Recurrence();
    recurrence->defaultRRule(true)->setRRule("RRULE:FREQ=MONTHLY;UNTIL=20150521");
    format.fromString(recurrence->defaultRRule(), "FREQ=MONTHLY;UNTIL=20150521");
    QTest::newRow("test2")
            << "9dnk2k6bspeoo19c95k9510reo"
            << false
            << KCalCore::Event::StatusConfirmed
            << KDateTime::fromString("2011-01-13T10:59:00.000Z", KDateTime::RFC3339Date)
            << KDateTime::fromString("2012-04-25T21:58:28.000Z", KDateTime::RFC3339Date)
            << QString("Cell phone payment")
            << QString("samoobsluha.vodafone.cz")
            << QString()
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KDateTime::fromString("2010-09-22T00:00:00+04:00", KDateTime::RFC3339Date)
            << KDateTime::fromString("2010-09-22T00:05:00+04:00", KDateTime::RFC3339Date)
            << false
            << recurrence
            << KCalCore::Event::Opaque
            << "9dnk2k6bspeoo19c95k9510reo@google.com"
            << 4
            << KCalCore::Attendee::List()
            << KCalCore::Alarm::List()
            << ByteArrayStringMap()
            << QString("{\n"
                "   \"kind\": \"calendar#event\",\n"
                "   \"etag\": \"\\\"Nj4ZG2eInHp1GWYvR0V2n_Z2u9Q/Q0tEM2pOenVKaEVCQUFBQUFBQUFBQT09\\\"\",\n"
                "   \"id\": \"9dnk2k6bspeoo19c95k9510reo\",\n"
                "   \"status\": \"confirmed\",\n"
                "   \"htmlLink\": \"https://www.google.com/calendar/event?eid=OWRuazJrNmJzcGVvbzE5Yzk1azk1MTByZW9fMjAxMDA5MjFUMjAwMDAwWiBkYW4udnJhdGlsQG0\",\n"
                "   \"created\": \"2011-01-13T10:59:00.000Z\",\n"
                "   \"updated\": \"2012-04-25T21:58:28.000Z\",\n"
                "   \"summary\": \"Cell phone payment\",\n"
                "   \"description\": \"samoobsluha.vodafone.cz\",\n"
                "   \"creator\": {\n"
                "       \"email\": \"dan.vratil@gmail.com\",\n"
                "       \"displayName\": \"Dan Vrátil\",\n"
                "       \"self\": true\n"
                "   },\n"
                "   \"organizer\": {\n"
                "       \"email\": \"dan.vratil@gmail.com\",\n"
                "       \"displayName\": \"Dan Vratil\",\n"
                "       \"self\": true\n"
                "   },\n"
                "   \"start\": {\n"
                "       \"dateTime\": \"2010-09-22T00:00:00+04:00\",\n"
                "       \"timeZone\": \"UTC\"\n"
                "   },\n"
                "   \"end\": {\n"
                "       \"dateTime\": \"2010-09-22T00:05:00+04:00\",\n"
                "       \"timeZone\": \"UTC\"\n"
                "   },\n"
                "   \"recurrence\": [\n"
                "       \"RRULE:FREQ=MONTHLY;UNTIL=20150521\"\n"
                "   ],\n"
                "   \"iCalUID\": \"9dnk2k6bspeoo19c95k9510reo@google.com\",\n"
                "   \"sequence\": 4,\n"
                "   \"reminders\": {\n"
                "       \"useDefault\": false\n"
                "   }\n"
                "}").toUtf8();
}

void ServicesCalendarTests::testJSONParser()
{
    KGAPI::Services::Calendar *service;
    KGAPI::Objects::Event *event;

    QFETCH(QString, id);
    QFETCH(KCalCore::Event::Status, status);
    QFETCH(KDateTime, created);
    QFETCH(KDateTime, updated);
    QFETCH(QString, summary);
    QFETCH(QString, description);
    QFETCH(QString, location);
    QFETCH(KCalCore::Person::Ptr, creator);
    QFETCH(KCalCore::Person::Ptr, organizer);
    QFETCH(KDateTime, dtStart);
    QFETCH(KDateTime, dtEnd);
    QFETCH(bool, allDay);
    QFETCH(KCalCore::Recurrence*, recurrence);
    QFETCH(KCalCore::Event::Transparency, transparency);
    QFETCH(KCalCore::Attendee::List, attendees);
    QFETCH(KCalCore::Alarm::List, reminders);
    QFETCH(ByteArrayStringMap, customProperties);
    QFETCH(QByteArray, jsonData);

    service = new KGAPI::Services::Calendar();
    event = dynamic_cast< KGAPI::Objects::Event* >(service->JSONToObject(jsonData));

    QCOMPARE(event->uid(), id);
    QCOMPARE(event->status(), status);
    QCOMPARE(event->created(), created);
    QCOMPARE(event->summary(), summary);
    QCOMPARE(event->description(), description);
    QCOMPARE(event->location(), location);
    /* We want to compare KCalCore::Persons, not pointers */
    QCOMPARE(*event->organizer(), *organizer);
    QCOMPARE(event->allDay(), allDay);

    if (allDay) {
        QCOMPARE(event->dtStart(), KDateTime(dtStart.date()));
        QCOMPARE(event->dtEnd(), KDateTime(dtEnd.date()));
    } else {
        QCOMPARE(event->dtStart(), dtStart);
        QCOMPARE(event->dtEnd(), dtEnd);
    }

    QCOMPARE(event->recurrence()->rRules().count(), recurrence->rRules().count());
    for (int i = 0; i < recurrence->rRules().count(); i++) {
        QCOMPARE(*event->recurrence()->rRules().at(i), *recurrence->rRules().at(i));
    };
    QCOMPARE(event->transparency(), transparency);
    QCOMPARE(event->attendees(), attendees);
    QCOMPARE(event->alarms(), reminders);
    QCOMPARE(event->customProperties(), customProperties);

    delete event;
    delete service;
}

void ServicesCalendarTests::testJSONSerializer_data()
{

}

void ServicesCalendarTests::testJSONSerializer()
{

}


void ServicesCalendarTests::testUrls()
{

}


QTEST_KDEMAIN(ServicesCalendarTests, NoGUI);