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

#include <QDateTime>
#include <KCalCore/Event>
#include <KCalCore/ICalFormat>
#include <KDebug>
#include <qjson/parser.h>

typedef QMap< QByteArray, QString > ByteArrayStringMap;

Q_DECLARE_METATYPE(KGAPI::Objects::Event*);
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
    QTest::addColumn< QDateTime >("created");
    QTest::addColumn< QDateTime >("updated");
    QTest::addColumn< QString >("summary");
    QTest::addColumn< QString >("description");
    QTest::addColumn< QString >("location");
    QTest::addColumn< KCalCore::Person::Ptr >("creator");
    QTest::addColumn< KCalCore::Person::Ptr >("organizer");
    QTest::addColumn< QDateTime >("dtStart");
    QTest::addColumn< QDateTime >("dtEnd");
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
            << QDateTime::fromString("2012-06-12T20:28:34.000Z", Qt::ISODate)
            << QDateTime::fromString("2012-06-12T20:28:34.000Z", Qt::ISODate)
            << QString("Den vzniku samostatného československého státu")
            << QString()
            << QString()
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << QDateTime::fromString("2012-10-28", QDateTime::ISODate)
            // Note the end-date correction! */
            << QDateTime::fromString("2012-10-28", QDateTime::ISODate)
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
            << QDateTime::fromString("2011-01-13T10:59:00.000Z", Qt::ISODate)
            << QDateTime::fromString("2012-04-25T21:58:28.000Z", Qt::ISODate)
            << QString("Cell phone payment")
            << QString("samoobsluha.vodafone.cz")
            << QString()
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << KCalCore::Person::Ptr(new KCalCore::Person())
            << QDateTime::fromString("2010-09-22T00:00:00+04:00", Qt::ISODate)
            << QDateTime::fromString("2010-09-22T00:05:00+04:00", Qt::ISODate)
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
                "       \"timeZone\": \"Europe/Moscow\"\n"
                "   },\n"
                "   \"end\": {\n"
                "       \"dateTime\": \"2010-09-22T00:05:00+04:00\",\n"
                "       \"timeZone\": \"Europe/Moscow\"\n"
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
    QFETCH(QDateTime, created);
    QFETCH(QDateTime, updated);
    QFETCH(QString, summary);
    QFETCH(QString, description);
    QFETCH(QString, location);
    QFETCH(KCalCore::Person::Ptr, creator);
    QFETCH(KCalCore::Person::Ptr, organizer);
    QFETCH(QDateTime, dtStart);
    QFETCH(QDateTime, dtEnd);
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
        QCOMPARE(event->dtStart(), QDateTime(dtStart.date()));
        QCOMPARE(event->dtEnd(), QDateTime(dtEnd.date()));
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
    QTest::addColumn< KGAPI::Objects::Event* >("event");

    KGAPI::Objects::Event *event = new KGAPI::Objects::Event();
    event->setUid("h@0203488aeaa000f7e787fef631d12bc0d7325ef2");
    event->setDeleted(false);
    event->setStatus(KCalCore::Incidence::StatusConfirmed);
    event->setCreated(QDateTime::fromString("2012-06-12T20:28:34.000Z", Qt::ISODate));
    event->setSummary("Den vzniku samostatného československého státu");
    event->setDtStart(QDateTime::fromString("2012-10-28", QDateTime::ISODate));
    event->setDtEnd(QDateTime::fromString("2012-10-28", QDateTime::ISODate));
    event->setAllDay(true);
    event->setTransparency(KCalCore::Event::Opaque);

    QTest::newRow("test1")
            << event;
}

void ServicesCalendarTests::testJSONSerializer()
{
    KGAPI::Services::Calendar service;

    QFETCH(KGAPI::Objects::Event*, event);

    QByteArray raw = service.objectToJSON(dynamic_cast< KGAPI::Object* >(event));
    QJson::Parser parser;
    bool ok;

    QVariant data = parser.parse(raw, &ok);
    QVERIFY(ok == true);

    QVariantMap map = data.toMap();

    QVERIFY(map.contains("type"));
    QVERIFY(map.contains("id"));
    QVERIFY(map.contains("status"));
    QVERIFY(map.contains("summary"));
    QVERIFY(map.contains("description"));
    QVERIFY(map.contains("location"));

    if (event->recurrence()->rRules().count() > 0) {
        QVERIFY(map.contains("recurrence"));
    } else {
        QVERIFY(! map.contains("recurrence"));
    }

    QVERIFY(map.contains("start"));
    QVariantMap start = map["start"].toMap();
    if (event->allDay()) {
        QVERIFY(start.contains("date"));
        QVERIFY( ! start.contains("dateTime"));
    } else {
        QVERIFY(start.contains("dateTime"));
        QVERIFY( ! start.contains("date"));
    }

    QVERIFY(map.contains("end"));
    QVariantMap end = map["end"].toMap();
    if (event->allDay()) {
        QVERIFY(end.contains("date"));
        QVERIFY( ! end.contains("dateTime"));
    } else {
        QVERIFY(end.contains("dateTime"));
        QVERIFY( ! end.contains("date"));
    }

    QVERIFY(map.contains("transparency"));
    if (event->attendeeCount() > 0) {
        QVERIFY(map.contains("attendees"));
    } else {
        QVERIFY( ! map.contains("attendees"));

        /* Check that event without attendees does not have
         * organizer set */
        QVERIFY( ! map.contains("organizer"));
    }

    QVERIFY(map.contains("reminders"));

    if (event->categories().count() > 0) {
        QVERIFY(map.contains("extendedProperties"));
    } else {
        QVERIFY( ! map.contains("extendedProperties"));
    }

    QCOMPARE(map["type"].toString(), QString("calendar#event"));
    QCOMPARE(map["id"].toString(), event->uid());
    QCOMPARE(map["summary"].toString(), event->summary());
    QCOMPARE(map["description"].toString(), event->description());
    QCOMPARE(map["location"].toString(), event->location());

    switch (event->status()) {
        case KCalCore::Incidence::StatusConfirmed:
            QCOMPARE(map["status"].toString(), QString("confirmed"));
            break;
        case KCalCore::Incidence::StatusCanceled:
            QCOMPARE(map["status"].toString(), QString("cancelled"));
            break;
        case KCalCore::Incidence::StatusTentative:
            QCOMPARE(map["status"].toString(), QString("tentative"));
            break;
        default:
            QFAIL(QString("Invalid incidence status ").append(map["status"].toString()).toLatin1().constData());
            break;
    }

    if (map.contains("recurrence")) {
        QVariantList recurrences = map["recurrence"].toList();
        QCOMPARE(recurrences.count(), event->recurrence()->rRules().count());
        for (int i = 0; i < recurrences.count(); i++) {
            QCOMPARE(recurrences.at(i).toString(),
                     event->recurrence()->rRules().at(i)->rrule());
        }
    }

    start = map["start"].toMap();
    if (event->allDay()) {
        QCOMPARE(start["date"].toString(), event->dtStart().toString(QDateTime::ISODate));
    } else {
        QCOMPARE(start["dateTime"].toString(), event->dtStart().toString(Qt::ISODate));
    }

    end = map["end"].toMap();
    if (event->allDay()) {
        /* Note the one-day correction in allday event ending! */
        QCOMPARE(end["date"].toString(), event->dtEnd().addDays(1).toString(QDateTime::ISODate));
    } else {
        QCOMPARE(end["dateTime"].toString(), event->dtEnd().toString(Qt::ISODate));
    }

    QCOMPARE(map["transparency"].toString(),
             ((event->transparency() == KCalCore::Event::Opaque) ?
                QString("opaque") : QString("transparent")));

    QVariantList attendees = map["attendees"].toList();
    QCOMPARE(attendees.count(), event->attendeeCount());
    for (int i = 0; i < attendees.count(); i++) {
        QVariantMap attendee = attendees.at(i).toMap();

        QCOMPARE(attendee["displayName"].toString(), event->attendees().at(i)->name());
        QCOMPARE(attendee["email"].toString(), event->attendees().at(i)->email());

        switch (event->attendees().at(i)->status()) {
            case KCalCore::Attendee::Accepted:
                QCOMPARE(attendee["responseStatus"].toString(), QString("accepted"));
                break;
            case KCalCore::Attendee::Declined:
                QCOMPARE(attendee["responseStatus"].toString(), QString("declined"));
                break;
            case KCalCore::Attendee::Tentative:
                QCOMPARE(attendee["responseStatus"].toString(), QString("tentative"));
                break;
            default:
                QCOMPARE(attendee["responseStatus"].toString(), QString("needsAction"));
                break;
        }

        QCOMPARE(attendee["optional"].toBool(),
                 (event->attendees().at(i)->role() == KCalCore::Attendee::OptParticipant));
    }

    if (!attendees.isEmpty() && !event->organizer()->isEmpty()) {
        QVariantMap organizer = map["organizer"].toMap();

        QCOMPARE(organizer["displayName"].toString(),
                 event->organizer()->fullName());
        QCOMPARE(organizer["email"].toString(),
                 event->organizer()->email());
    }

    QVariantMap reminders = map["reminders"].toMap();
    QCOMPARE(reminders["useDefault"].toBool(), false);

    QVariantList overrides = reminders["overrides"].toList();
    QCOMPARE(overrides.count(), event->alarms().count());
    for (int i = 0; i < overrides.count(); i++) {
        QVariantMap override = overrides.at(i).toMap();

        QCOMPARE(override["minutes"].toInt(),
                 (int)(event->alarms().at(i)->startOffset().asSeconds()));
        switch (event->alarms().at(i)->type()) {
            case KCalCore::Alarm::Display:
                QCOMPARE(override["method"].toString(), QString("popup"));
                break;
            case KCalCore::Alarm::Email:
                QCOMPARE(override["method"].toString(), QString("email"));
                break;
            default:
                QFAIL("Alarm of type other then Display or Email defined!");
                break;
        }
    }

    if (map.contains("extendedProperties")) {
        QVariantMap extendedProperties = map["extendedProperties"].toMap();
        QVERIFY(extendedProperties.contains("shared"));

        QVariantMap sharedProperties = extendedProperties["shared"].toMap();
        QVERIFY(sharedProperties.contains("categories"));

        QCOMPARE(sharedProperties["categories"].toString(),
                 event->categoriesStr());
    }
}


void ServicesCalendarTests::testUrls()
{
    QCOMPARE(KGAPI::Services::Calendar::fetchCalendarsUrl().toString(),
             QString("https://www.googleapis.com/calendar/v3/users/me/calendarList"));
    QCOMPARE(KGAPI::Services::Calendar::fetchCalendarUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/users/me/calendarList/1234abcd"));
    QCOMPARE(KGAPI::Services::Calendar::updateCalendarUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd"));
    QCOMPARE(KGAPI::Services::Calendar::createCalendarUrl().toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars"));
    QCOMPARE(KGAPI::Services::Calendar::updateCalendarUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd"));
    QCOMPARE(KGAPI::Services::Calendar::removeCalendarUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd"));
    QCOMPARE(KGAPI::Services::Calendar::fetchEventsUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events?maxResults=20"));
    QCOMPARE(KGAPI::Services::Calendar::fetchEventUrl("1234abcd", "5678efgh").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events/5678efgh"));
    QCOMPARE(KGAPI::Services::Calendar::createEventUrl("1234abcd").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events"));
    QCOMPARE(KGAPI::Services::Calendar::updateEventUrl("1234abcd", "5678efgh").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events/5678efgh"));
    QCOMPARE(KGAPI::Services::Calendar::removeEventUrl("1234abcd", "5678efgh").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events/5678efgh"));
    QCOMPARE(KGAPI::Services::Calendar::moveEventUrl("1234abcd", "dcba4321", "5678efgh").toString(),
             QString("https://www.googleapis.com/calendar/v3/calendars/1234abcd/events/5678efgh?destination=dcba4321"));
}


QTEST_KDEMAIN(ServicesCalendarTests, NoGUI);
