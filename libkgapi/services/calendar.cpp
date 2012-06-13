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

#include "calendar.h"
#include "accessmanager.h"
#include "objects/calendar.h"
#include "objects/event.h"

#ifdef WITH_KCAL
#include <KCal/Event>
#include <KCal/Attendee>
#include <KCal/Recurrence>
#include <KCal/RecurrenceRule>
#include <KCal/ICalFormat>
using namespace KCal;
#else
#include <KCalCore/Event>
#include <KCalCore/Attendee>
#include <KCalCore/Recurrence>
#include <KCalCore/RecurrenceRule>
#include <KCalCore/ICalFormat>
using namespace KCalCore;
#endif

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <KSystemTimeZones>
#include <KUrl>

#include <QtCore/QVariant>

namespace KGAPI
{

namespace Services
{

class CalendarPrivate
{

public:
    static DateList parseRDate(const QString &rule);

    static KGAPI::Object* JSONToCalendar(const QVariantMap& calendar);
    static QVariantMap calendarToJSON(KGAPI::Object* calendar);
    static QList<KGAPI::Object*> parseCalendarJSONFeed(const QVariantList& feed);

    static KGAPI::Object* JSONToEvent(const QVariantMap& event);
    static QVariantMap eventToJSON(KGAPI::Object *event);
    static QList<KGAPI::Object*> parseEventJSONFeed(const QVariantList& feed);
};

}

}

using namespace KGAPI;

QUrl Services::Calendar::ScopeUrl("https://www.googleapis.com/auth/calendar");

/********** PUBLIC JSON INTERFACE ************/

KGAPI::Object* Services::Calendar::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;

    QVariantMap object = parser.parse(jsonData).toMap();

    if ((object["kind"] == "calendar#calendarListEntry") || (object["kind"] == "calendar#calendar")) {
        return CalendarPrivate::JSONToCalendar(object);
    } else if (object["kind"] == "calendar#event") {
        return CalendarPrivate::JSONToEvent(object);
    }

    return 0;
}

QByteArray Services::Calendar::objectToJSON(KGAPI::Object* object)
{
    QVariantMap map;

    if (dynamic_cast< const Objects::Calendar* >(object)) {
        map = CalendarPrivate::calendarToJSON(object);
    } else if (dynamic_cast< const Objects::Event* >(object)) {
        map = CalendarPrivate::eventToJSON(object);
    }

    QJson::Serializer serializer;
    return serializer.serialize(map);
}

QList< KGAPI::Object* > Services::Calendar::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QVariantMap data = parser.parse(jsonFeed).toMap();

    QList< KGAPI::Object* > list;

    if (data["kind"] == "calendar#calendarList") {
        list = CalendarPrivate::parseCalendarJSONFeed(data["items"].toList());

        if (data.contains("nextPageToken")) {
            feedData.nextPageUrl = fetchCalendarsUrl();
            feedData.nextPageUrl.addQueryItem("pageToken", data["nextPageToken"].toString());
            if (feedData.nextPageUrl.queryItemValue("maxResults").isEmpty()) {
                feedData.nextPageUrl.addQueryItem("maxResults","20");
            }
        }

    } else if (data["kind"] == "calendar#events") {
        list = CalendarPrivate::parseEventJSONFeed(data["items"].toList());
        if (data.contains("nextPageToken")) {
            QString calendarId = feedData.requestUrl.toString().remove("https://www.googleapis.com/calendar/v3/calendars/");
            calendarId = calendarId.left(calendarId.indexOf("/"));
            feedData.nextPageUrl = fetchEventsUrl(calendarId);
            feedData.nextPageUrl.addQueryItem("pageToken", data["nextPageToken"].toString());
            if (feedData.nextPageUrl.queryItemValue("maxResults").isEmpty()) {
                feedData.nextPageUrl.addQueryItem("maxResults","20");
            }
        }
    }

    return list;
}


/************* PUBLIC XML INTERFACE ***********/

KGAPI::Object* Services::Calendar::XMLToObject(const QByteArray& xmlData)
{
    Q_UNUSED(xmlData);

    return 0;
}

QByteArray Services::Calendar::objectToXML(KGAPI::Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
}

QList< KGAPI::Object* > Services::Calendar::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}


/************* URLS **************/

const QUrl& Services::Calendar::scopeUrl() const
{
    return Services::Calendar::ScopeUrl;
}

QUrl Services::Calendar::fetchCalendarsUrl()
{
    return QUrl("https://www.googleapis.com/calendar/v3/users/me/calendarList");
}

QUrl Services::Calendar::fetchCalendarUrl(const QString& calendarID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/users/me/calendarList/");
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::updateCalendarUrl(const QString &calendarID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::createCalendarUrl()
{
    return QUrl("https://www.googleapis.com/calendar/v3/calendars");
}

QUrl Services::Calendar::removeCalendarUrl(const QString& calendarID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::fetchEventsUrl(const QString& calendarID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);
    url.addPath("events");
    url.addQueryItem("maxResults", "20");

    return url;
}

QUrl Services::Calendar::fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);
    url.addPath("events");
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::updateEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);
    url.addPath("events");
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::createEventUrl(const QString& calendarID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);
    url.addPath("events");

    return url;
}

QUrl Services::Calendar::removeEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(calendarID);
    url.addPath("events");
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::moveEventUrl(const QString& sourceCalendar, const QString& destCalendar, const QString& eventID)
{
    KUrl url("https://www.googleapis.com/calendar/v3/calendars/");
    url.addPath(sourceCalendar);
    url.addPath("events");
    url.addPath(eventID);
    url.addQueryItem("destination", destCalendar);

    return url;
}


QString Services::Calendar::protocolVersion() const
{
    return "3";
}

bool Services::Calendar::supportsJSONRead(QString* urlParam)
{
    return true;

    Q_UNUSED(urlParam)
}

bool Services::Calendar::supportsJSONWrite(QString* urlParam)
{
    return true;

    Q_UNUSED(urlParam)
}


/******************************** PRIVATE ***************************************/

KGAPI::Object* Services::CalendarPrivate::JSONToCalendar(const QVariantMap& calendar)
{
    Objects::Calendar *object = new Objects::Calendar();

    QString id = QUrl::fromPercentEncoding(calendar["id"].toByteArray());
    object->setUid(id);
    object->setEtag(calendar["etag"].toString());
    object->setTitle(calendar["summary"].toString());
    object->setDetails(calendar["description"].toString());
    object->setLocation(calendar["location"].toString());
    object->setTimezone(calendar["timeZone"].toString());

    if ((calendar["accessRole"].toString() == "writer") || (calendar["acessRole"].toString() == "owner")) {
        object->setEditable(true);
    } else {
        object->setEditable(false);
    }

    QVariantList reminders = calendar["defaultReminders"].toList();
    Q_FOREACH(const QVariant & r, reminders) {
        QVariantMap reminder = r.toMap();

        Objects::Reminder::Ptr rem(new Objects::Reminder());
        if (reminder["method"].toString() == "email") {
            rem->setType(Alarm::Email);
        } else if (reminder["method"].toString() == "popup") {
            rem->setType(Alarm::Display);
        } else {
            rem->setType(Alarm::Invalid);
        }

        rem->setStartOffset(Duration(reminder["minutes"].toInt() * (-60)));

        object->addDefaultReminer(rem);
    }

    return dynamic_cast< KGAPI::Object* >(object);
}

QVariantMap Services::CalendarPrivate::calendarToJSON(KGAPI::Object* calendar)
{
    QVariantMap output, entry;
    Objects::Calendar *object = static_cast< Objects::Calendar* >(calendar);

    if (!object->uid().isEmpty()) {
        entry["id"] = object->uid();
    }

    entry["summary"] = object->title();
    entry["description"] = object->details();
    entry["location"] = object->location();
    if (!object->timezone().isEmpty()) {
        entry["timeZone"] = object->timezone();
    }

    return entry;
}

QList< KGAPI::Object* > Services::CalendarPrivate::parseCalendarJSONFeed(const QVariantList& feed)
{
    QList< KGAPI::Object* > output;

    Q_FOREACH(const QVariant &i, feed) {
        output.append(CalendarPrivate::JSONToCalendar(i.toMap()));
    }

    return output;
}

KGAPI::Object* Services::CalendarPrivate::JSONToEvent(const QVariantMap& event)
{
    Objects::Event *object = new Objects::Event();

    /* ID */
    object->setUid(QUrl::fromPercentEncoding(event["id"].toByteArray()));

    /* ETAG */
    object->setEtag(event["etag"].toString());

    /* Status */
    if (event["status"].toString() == "confirmed") {
        object->setStatus(Incidence::StatusConfirmed);
    } else if (event["status"].toString() == "cancelled") {
        object->setStatus(Incidence::StatusCanceled);
        object->setDeleted(true);
    } else if (event["status"].toString() == "tentative") {
        object->setStatus(Incidence::StatusTentative);
    } else {
        object->setStatus(Incidence::StatusNone);
    }

    /* Canceled instance of recurring event. Set ID of the instance to match ID of the event */
    if (event.contains("recurringEventId") && object->deleted()) {
        object->setUid(QUrl::fromPercentEncoding(event["recurringEventId"].toByteArray()));
    }

    /* Created */
    object->setCreated(AccessManager::RFC3339StringToDate(event["created"].toString()));

    /* Last updated */
    object->setLastModified(AccessManager::RFC3339StringToDate(event["updated"].toString()));

    /* Summary */
    object->setSummary(event["summary"].toString());

    /* Description */
    object->setDescription(event["description"].toString());

    /* Location */
    object->setLocation(event["location"].toString());

    /* Start date */
    QVariantMap startData = event["start"].toMap();
    KDateTime dtStart;
    if (startData.contains("date")) {
        dtStart = KDateTime::fromString(startData["date"].toString(), KDateTime::ISODate);
        object->setAllDay(true);
    } else if (startData.contains("dateTime")) {
        dtStart = AccessManager::RFC3339StringToDate(startData["dateTime"].toString());
    }
    if (startData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(startData["timeZone"].toString());
        dtStart.setTimeSpec(KDateTime::Spec(tz));
    }
    object->setDtStart(dtStart.toLocalZone());

    /* End date */
    QVariantMap endData = event["end"].toMap();
    KDateTime dtEnd;
    if (endData.contains("date")) {
        dtEnd = KDateTime::fromString(endData["date"].toString(), KDateTime::ISODate);
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        dtEnd = dtEnd.addDays(-1);
        object->setAllDay(true);
    } else if (endData.contains("dateTime")) {
        dtEnd = AccessManager::RFC3339StringToDate(endData["dateTime"].toString());
    }
    if (endData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(endData["timeZone"].toString());
        dtEnd.setTimeSpec(KDateTime::Spec(tz));
    }
    object->setDtEnd(dtEnd);

    /* Transparency */
    if (event["transparency"].toString() == "transparent") {
        object->setTransparency(Event::Transparent);
    } else { /* Assume opaque as default transparency */
        object->setTransparency(Event::Opaque);
    }

    /* Attendees */
    QVariantList attendees = event["attendees"].toList();
    Q_FOREACH(const QVariant & a, attendees) {
        QVariantMap att = a.toMap();
        AttendeePtr attendee(
            new Attendee(att["displayName"].toString(),
                         att["email"].toString()));

        if (att["responseStatus"].toString() == "accepted")
            attendee->setStatus(Attendee::Accepted);
        else if (att["responseStatus"].toString() == "declined")
            attendee->setStatus(Attendee::Declined);
        else if (att["responseStatus"].toString() == "tentative")
            attendee->setStatus(Attendee::Tentative);
        else
            attendee->setStatus(Attendee::NeedsAction);

        if (att["optional"].toBool()) {
            attendee->setRole(Attendee::OptParticipant);
        }

        object->addAttendee(attendee, true);
    }

    /* According to RFC, only events with attendees can have an organizer.
     * Google seems to ignore it, so we must take care of it here */
    if (object->attendeeCount() > 0) {
        PersonPtr organizer(new Person);
        QVariantMap organizerData = event["organizer"].toMap();
        organizer->setName(organizerData["displayName"].toString());
        organizer->setEmail(organizerData["email"].toString());
    #ifdef WITH_KCAL
        object->setOrganizer(*organizer);
    #else
        object->setOrganizer(organizer);
    #endif
    }

    /* Recurrence */
    QStringList recrs = event["recurrence"].toStringList();
    Q_FOREACH(const QString & rec, recrs) {
        ICalFormat format;
        if (rec.left(5) == "RRULE") {
            RecurrenceRule *recurrenceRule = new RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(6));
            object->recurrence()->addRRule(recurrenceRule);
        } else if (rec.left(6) == "EXRULE") {
            RecurrenceRule *recurrenceRule = new RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(7));
            object->recurrence()->addExRule(recurrenceRule);
        } else if (rec.left(6) == "EXDATE") {
            DateList exdates = parseRDate(rec);
            object->recurrence()->setExDates(exdates);
        } else if (rec.left(5) == "RDATE") {
            DateList rdates = parseRDate(rec);
            object->recurrence()->setRDates(rdates);
        }
    }

    QVariantMap reminders = event["reminders"].toMap();
    if (reminders.contains("useDefault") && reminders["useDefault"].toBool()) {
        object->setUseDefaultReminders(true);
    } else {
        object->setUseDefaultReminders(false);
    }

    QVariantList overrides = reminders["overrides"].toList();
    Q_FOREACH(const QVariant & r, overrides) {
        QVariantMap override = r.toMap();
        AlarmPtr alarm(new Alarm(object));
        alarm->setTime(object->dtStart());

        if (override["method"].toString() == "popup") {
            alarm->setType(Alarm::Display);
        } else if (override["method"].toString() == "email") {
            alarm->setType(Alarm::Email);
        } else {
            alarm->setType(Alarm::Invalid);
            continue;
        }

        alarm->setStartOffset(Duration(override["minutes"].toInt() * (-60)));
        alarm->setEnabled(true);
        object->addAlarm(alarm);
    }

    /* Extended properties */
    QVariantMap extendedProperties = event["extendedProperties"].toMap();

    QVariantMap privateProperties = extendedProperties["private"].toMap();
    QMap< QString, QVariant >::const_iterator iter = privateProperties.begin();
    while (iter != privateProperties.end()) {
        if (iter.key() == "categories") {
            object->setCategories(iter.value().toString());
        }

        ++iter;
    }

    QVariantMap sharedProperties = extendedProperties["shared"].toMap();
    iter = sharedProperties.begin();
    while (iter != sharedProperties.end()) {
        if (iter.key() == "categories") {
            object->setCategories(iter.value().toString());
        }

        ++iter;
    }

    return dynamic_cast< KGAPI::Object* >(object);
}

QVariantMap Services::CalendarPrivate::eventToJSON(KGAPI::Object* event)
{
    Objects::Event *object = static_cast<Objects::Event*>(event);
    QVariantMap output, data;

    /* Type */
    data["type"] = "calendar#event";

    /* ID */
    if (!object->uid().isEmpty())
        data["id"] = object->uid();

    /* Status */
    if (object->status() == Incidence::StatusConfirmed) {
        data["status"] = "confirmed";
    } else if (object->status() == Incidence::StatusCanceled) {
        data["status"] = "canceled";
    } else if (object->status() == Incidence::StatusTentative) {
        data["status"] = "tentative";
    }

    /* Summary */
    data["summary"] = object->summary();

    /* Description */
    data["description"] = object->description();

    /* Location */
    data["location"] = object->location();

    /* Recurrence */
    QVariantList recurrence;
    ICalFormat format;
    Q_FOREACH(RecurrenceRule * rRule, object->recurrence()->rRules()) {
        recurrence << format.toString(rRule).remove("\r\n");
    }

    Q_FOREACH(RecurrenceRule * rRule, object->recurrence()->exRules()) {
        recurrence << format.toString(rRule).remove("\r\n");
    }

    QStringList dates;
    Q_FOREACH(const QDate & rDate, object->recurrence()->rDates()) {
        dates << rDate.toString("yyyyMMdd");
    }

    if (!dates.isEmpty()) {
        recurrence << "RDATE;VALUE=DATA:" + dates.join(",");
    }

    dates.clear();
    Q_FOREACH(const QDate & exDate, object->recurrence()->exDates()) {
        dates << exDate.toString("yyyyMMdd");
    }

    if (!dates.isEmpty()) {
        recurrence << "EXDATE;VALUE=DATE:" + dates.join(",");
    }

    if (!recurrence.isEmpty()) {
        data["recurrence"] = recurrence;
    }

    /* Start */
    QVariantMap start;
    if (object->allDay()) {
        start["date"] = object->dtStart().toString("%Y-%m-%d");
    } else {
        start["dateTime"] = AccessManager::dateToRFC3339String(object->dtStart());
    }
    QString tzStart = object->dtStart().timeZone().name();
    if (!recurrence.isEmpty() && tzStart.isEmpty()) {
        tzStart = KTimeZone::utc().name();
    }
    if (!tzStart.isEmpty()) {
        start["timeZone"] = tzStart;
    }
    data["start"] = start;

    /* End */
    QVariantMap end;
    if (object->allDay()) {
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        KDateTime dtEnd = object->dtEnd().addDays(1);
        end["date"] = dtEnd.toString("%Y-%m-%d");
    } else {
        end["dateTime"] = AccessManager::dateToRFC3339String(object->dtEnd());
    }
    QString tzEnd = object->dtEnd().timeZone().name();
    if (!recurrence.isEmpty() && tzEnd.isEmpty()) {
        tzEnd = KTimeZone::utc().name();
    }
    if (!tzEnd.isEmpty()) {
        end["timeZone"] = tzEnd;
    }
    data["end"] = end;

    /* Transparency */
    if (object->transparency() == Event::Transparent)
        data["transparency"] = "transparent";
    else
        data["transparency"] = "opaque";

    /* Attendees */
    QVariantList atts;
    Q_FOREACH(const AttendeePtr attee, object->attendees()) {
        QVariantMap att;

        att["displayName"] = attee->name();
        att["email"] = attee->email();

        if (attee->status() == Attendee::Accepted)
            att["responseStatus"] = "accepted";
        else if (attee->status() == Attendee::Declined)
            att["responseStatus"] = "declined";
        else if (attee->status() == Attendee::Tentative)
            att["responseStatus"] = "tentative";
        else
            att["responseStatus"] = "needsAction";

        if (attee->role() == Attendee::OptParticipant) {
            att["optional"] = true;
        }

        atts.append(att);
    }

    if (!atts.isEmpty()) {
        data["attendees"] = atts;

        /* According to RFC, event without attendees should not have
         * any organizer. */
    #ifdef WITH_KCAL
        PersonPtr organizer = new Person(object->organizer());
    #else
        PersonPtr organizer = object->organizer();
    #endif
        if (!organizer->isEmpty()) {
            QVariantMap org;
            org["displayName"] = organizer->fullName();
            org["email"] = organizer->email();
            data["organizer"] = org;
        }
    }

    /* Reminders */
    QVariantList overrides;
    Q_FOREACH(AlarmPtr alarm, object->alarms()) {
        QVariantMap override;

        if (alarm->type() == Alarm::Display) {
            override["method"] = "popup";
        } else if (alarm->type() == Alarm::Email) {
            override["method"] = "email";
        } else {
            continue;
        }

        override["minutes"] = (int)(alarm->startOffset().asSeconds() / -60);

        overrides << override;
    }

    QVariantMap reminders;
    reminders["useDefault"] = false;
    reminders["overrides"] = overrides;
    data["reminders"] = reminders;

    /* Store categories */
    if (!object->categories().isEmpty()) {
        QVariantMap extendedProperties;
        QVariantMap sharedProperties;
        sharedProperties["categories"] = object->categoriesStr();
        extendedProperties["shared"] = sharedProperties;
        data["extendedProperties"] = extendedProperties;
    }

    /* TODO: Implement support for additional features:
     * http://code.google.com/apis/gdata/docs/2.0/elements.html
     */

    return data;
}

QList< KGAPI::Object* > Services::CalendarPrivate::parseEventJSONFeed(const QVariantList& feed)
{
    QList< KGAPI::Object* > output;

    Q_FOREACH(const QVariant &i, feed) {
        output.append(JSONToEvent(i.toMap()));
    }

    return output;
}

DateList Services::CalendarPrivate::parseRDate(const QString& rule)
{
    DateList list;
    QString value;
    KTimeZone tz;

    QString left = rule.left(rule.indexOf(':'));
    QStringList params = left.split(';');
    Q_FOREACH(const QString & param, params) {
        if (param.startsWith(QLatin1String("VALUE"))) {
            value = param.mid(param.indexOf('=') + 1);
        } else if (param.startsWith(QLatin1String("TZID"))) {
            QString tzname = param.mid(param.indexOf('=') + 1);
            tz = KSystemTimeZones::zone(tzname);
        }
    }

    QString datesStr = rule.mid(rule.lastIndexOf(":") + 1);
    QStringList dates = datesStr.split(',');
    Q_FOREACH(const QString &date, dates) {
        QDate dt;

        if (value == "DATE") {
            dt = QDate::fromString(date, "yyyyMMdd");
        } else if (value == "PERIOD") {
            QString start = date.left(date.indexOf('/'));
            KDateTime kdt = AccessManager::RFC3339StringToDate(start);
            if (tz.isValid()) {
                kdt.setTimeSpec(tz);
            }

            dt = kdt.date();
        } else {
            KDateTime kdt = AccessManager::RFC3339StringToDate(date);
            if (tz.isValid()) {
                kdt.setTimeSpec(tz);
            }

            dt = kdt.date();
        }

        list << dt;
    }

    return list;
}
