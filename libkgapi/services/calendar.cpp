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

    /**
     * Checks whether TZID is in Olson format and converts it to it if neccessary
     *
     * This is mainly to handle crazy Microsoft TZIDs like
     * "(GMT) Greenwich Mean Time/Dublin/Edinburgh/London", because Google only
     * accepts TZIDs in Olson format ("Europe/London").
     *
     * It first tries to match the given \p tzid to all TZIDs in KTimeZones::zones().
     * If it fails, it parses the \p event, looking for X-MICROSOFT-CDO-TZID
     * property and than matches it to Olson-formatted TZID using a table.
     *
     * When the method fails to process the TZID, it returns the original \p tzid
     * in hope, that Google will cope with it.
     */
    static QString checkAndConverCDOTZID(const QString &tzid, const Objects::Event *event);
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

    if ((calendar["accessRole"].toString() == "writer") || (calendar["accessRole"].toString() == "owner")) {
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
    if (event.contains("recurringEventId")) {
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
        start["timeZone"] = checkAndConverCDOTZID(tzStart, object);
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
        end["timeZone"] = checkAndConverCDOTZID(tzEnd, object);
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


static QMap<int, QString> initMSCDOTZIDTable()
{
    QMap<int, QString> map;

    /* Based on "Time Zone to CdoTimeZoneId Map"
     * http://msdn.microsoft.com/en-us/library/aa563018%28loband%29.aspx
     *
     * The mapping is not exact, since the CdoTimeZoneId usually refers to a
     * region of multiple countries, so I always picked one of the countries
     * in the specified region and used it's TZID.
     */
    map.insert(0,  QLatin1String("UTC"));
    map.insert(1,  QLatin1String("Europe/London"));                       /* GMT Greenwich Mean Time; Dublin, Edinburgh, London */
    /* Seriously? *sigh* Let's handle these two in checkAndConvertCDOTZID() */
    //map.insertMulti(2, QLatin1String("Europe/Lisbon"));         /* GMT Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    //map.insertMulti(2, QLatin1String("Europe/Sarajevo"));       /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
    map.insert(3,  QLatin1String("Europe/Paris"));              /* GMT+01:00 Paris, Madrid, Brussels, Copenhagen */
    map.insert(4,  QLatin1String("Europe/Berlin"));             /* GMT+01:00 Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    map.insert(5,  QLatin1String("Europe/Bucharest"));          /* GMT+02:00 Bucharest */
    map.insert(6,  QLatin1String("Europe/Prague"));             /* GMT+01:00 Prague, Central Europe */
    map.insert(7,  QLatin1String("Europe/Athens"));             /* GMT+02:00 Athens, Istanbul, Minsk */
    map.insert(8,  QLatin1String("America/Brazil"));            /* GMT-03:00 Brasilia */
    map.insert(9,  QLatin1String("America/Halifax"));           /* GMT-04:00 Atlantic time (Canada) */
    map.insert(10, QLatin1String("America/New_York"));          /* GMT-05:00 Eastern Time (US & Canada) */
    map.insert(11, QLatin1String("America/Chicago"));           /* GMT-06:00 Central Time (US & Canada) */
    map.insert(12, QLatin1String("America/Denver"));            /* GMT-07:00 Mountain Time (US & Canada) */
    map.insert(13, QLatin1String("America/Los_Angeles"));       /* GMT-08:00 Pacific Time (US & Canada); Tijuana */
    map.insert(14, QLatin1String("America/Anchorage"));         /* GMT-09:00 Alaska */
    map.insert(15, QLatin1String("Pacific/Honolulu"));          /* GMT-10:00 Hawaii */
    map.insert(16, QLatin1String("Pacific/Apia"));              /* GMT-11:00 Midway Island, Samoa */
    map.insert(17, QLatin1String("Pacific/Auckland"));          /* GMT+12:00 Auckland, Wellington */
    map.insert(18, QLatin1String("Australia/Brisbane"));        /* GMT+10:00 Brisbane, East Australia */
    map.insert(19, QLatin1String("Australia/Adelaide"));        /* GMT+09:30 Adelaide, Central Australia */
    map.insert(20, QLatin1String("Asia/Tokyo"));                /* GMT+09:00 Osaka, Sapporo, Tokyo */
    map.insert(21, QLatin1String("Asia/Singapore"));            /* GMT+08:00 Kuala Lumpur, Singapore */
    map.insert(22, QLatin1String("Asia/Bangkok"));              /* GMT+07:00 Bangkok, Hanoi, Jakarta */
    map.insert(23, QLatin1String("Asia/Calcutta"));             /* GMT+05:30 Kolkata, Chennai, Mumbai, New Delhi, India Standard Time */
    map.insert(24, QLatin1String("Asia/Dubai"));                /* GMT+04:00 Abu Dhabi, Muscat */
    map.insert(25, QLatin1String("Asia/Tehran"));               /* GMT+03:30 Tehran */
    map.insert(26, QLatin1String("Asia/Baghdad"));              /* GMT+03:00 Baghdad */
    map.insert(27, QLatin1String("Asia/Jerusalem"));            /* GMT+02:00 Israel, Jerusalem Standard Time */
    map.insert(28, QLatin1String("America/St_Johns"));          /* GMT-03:30 Newfoundland */
    map.insert(29, QLatin1String("Atlantic/Portugal"));         /* GMT-01:00 Azores */
    map.insert(30, QLatin1String("America/Noronha"));           /* GMT-02:00 Mid-Atlantic */
    map.insert(31, QLatin1String("Africa/Monrovia"));           /* GMT Casablanca, Monrovia */
    map.insert(32, QLatin1String("America/Argentina/Buenos_Aires")); /* GMT-03:00 Buenos Aires, Georgetown */
    map.insert(33, QLatin1String("America/La_Paz"));            /* GMT-04:00 Caracas, La Paz */
    map.insert(34, QLatin1String("America/New_York"));          /* GMT-05:00 Indiana (East) */
    map.insert(35, QLatin1String("America/Bogota"));            /* GMT-05:00 Bogota, Lima, Quito */
    map.insert(36, QLatin1String("America/Winnipeg"));          /* GMT-06:00 Saskatchewan */
    map.insert(37, QLatin1String("America/Mexico_City"));       /* GMT-06:00 Mexico City, Tegucigalpa */
    map.insert(38, QLatin1String("America/Phoenix"));           /* GMT-07:00 Arizona */
    map.insert(39, QLatin1String("Pacific/Kwajalein"));         /* GMT-12:00 Eniwetok, Kwajalein, Dateline Time */
    map.insert(40, QLatin1String("Pacific/Fiji"));              /* GMT+12:00 Fušál, Kamchatka, Mashall Is. */
    map.insert(41, QLatin1String("Pacific/Noumea"));            /* GMT+11:00 Magadan, Solomon Is., New Caledonia */
    map.insert(42, QLatin1String("Australia/Hobart"));          /* GMT+10:00 Hobart, Tasmania */
    map.insert(43, QLatin1String("Pacific/Guam"));              /* GMT+10:00 Guam, Port Moresby */
    map.insert(44, QLatin1String("Australia/Darwin"));          /* GMT+09:30 Darwin */
    map.insert(45, QLatin1String("Asia/Shanghai"));             /* GMT+08:00 Beijing, Chongqing, Hong Kong SAR, Urumqi */
    map.insert(46, QLatin1String("Asia/Omsk"));                 /* GMT+06:00 Almaty, Novosibirsk, North Central Asia */
    map.insert(47, QLatin1String("Asia/Karachi"));              /* GMT+05:00 Islamabad, Karachi, Tashkent */
    map.insert(48, QLatin1String("Asia/Kabul"));                /* GMT+04:30 Kabul */
    map.insert(49, QLatin1String("Africa/Cairo"));              /* GMT+02:00 Cairo */
    map.insert(50, QLatin1String("Africa/Harare"));             /* GMT+02:00 Harare, Pretoria */
    map.insert(51, QLatin1String("Europe/Moscow"));             /* GMT+03:00 Moscow, St. Petersburg, Volgograd */
    map.insert(53, QLatin1String("Atlantic/Cape_Verde"));       /* GMT-01:00 Cape Verde Is. */
    map.insert(54, QLatin1String("Asia/Tbilisi"));              /* GMT+04:00 Baku, Tbilisi, Yerevan */
    map.insert(55, QLatin1String("America/Tegucigalpa"));       /* GMT-06:00 Central America */
    map.insert(56, QLatin1String("Africa/Nairobi"));            /* GMT+03:00 East Africa, Nairobi */
    map.insert(58, QLatin1String("Asia/Yekaterinburg"));        /* GMT+05:00 Ekaterinburg */
    map.insert(59, QLatin1String("Europe/Helsinki"));           /* GMT+02:00 Helsinki, Riga, Tallinn */
    map.insert(60, QLatin1String("America/Greenland"));         /* GMT-03:00 Greenland */
    map.insert(61, QLatin1String("Asia/Rangoon"));              /* GMT+06:30 Yangon (Rangoon) */
    map.insert(62, QLatin1String("Asia/Katmandu"));             /* GMT+05:45 Kathmandu, Nepal */
    map.insert(63, QLatin1String("Asia/Irkutsk"));              /* GMT+08:00 Irkutsk, Ulaan Bataar */
    map.insert(64, QLatin1String("Asia/Krasnoyarsk"));          /* GMT+07:00 Krasnoyarsk */
    map.insert(65, QLatin1String("America/Santiago"));          /* GMT-04:00 Santiago */
    map.insert(66, QLatin1String("Asia/Colombo"));              /* GMT+06:00 Sri Jayawardenepura, Sri Lanka */
    map.insert(67, QLatin1String("Pacific/Tongatapu"));         /* GMT+13:00 Nuku'alofa, Tonga */
    map.insert(68, QLatin1String("Asia/Vladivostok"));          /* GMT+10:00 Vladivostok */
    map.insert(69, QLatin1String("Africa/Bangui"));             /* GMT+01:00 West Central Africa */
    map.insert(70, QLatin1String("Asia/Yakutsk"));              /* GMT+09:00 Yakutsk */
    map.insert(71, QLatin1String("Asia/Dhaka"));                /* GMT+06:00 Astana, Dhaka */
    map.insert(72, QLatin1String("Asia/Seoul"));                /* GMT+09:00 Seoul, Korea Standard time */
    map.insert(73, QLatin1String("Australia/Perth"));           /* GMT+08:00 Perth, Western Australia */
    map.insert(74, QLatin1String("Asia/Kuwait"));               /* GMT+03:00 Arab, Kuwait, Riyadh */
    map.insert(75, QLatin1String("Asia/Taipei"));               /* GMT+08:00 Taipei */
    map.insert(76, QLatin1String("Australia/Sydney"));          /* GMT+10:00 Canberra, Melbourne, Sydney */

    return map;
}

static const QMap<int, QString> MSCDOTZIDTable = initMSCDOTZIDTable();

QString Services::CalendarPrivate::checkAndConverCDOTZID(const QString& tzid, const Objects::Event* event)
{
    /* Try to match the @tzid to any valid timezone we know. */
    KTimeZones timeZones;
    const KTimeZones::ZoneMap zones = timeZones.zones();
    KTimeZones::ZoneMap::const_iterator iter;
    for (iter = zones.constBegin(); iter != zones.constEnd(); iter++) {
        if (iter.key() == tzid) {
            /* Yay, @tzid is a valid TZID in Olson format */
            return tzid;
        }
    }

    /* Damn, no match. Parse the iCal and try to find X-MICROSOFT-CDO-TZID
     * property that we can match against the MSCDOTZIDTable */
    ICalFormat format;
    /* Use a copy of @event, otherwise it would be deleted when ptr is destroyed */
    Incidence::Ptr ptr(new Event(*dynamic_cast<const Event*>(event)));
    const QString vcard = format.toICalString(ptr);
    const QStringList properties = vcard.split(QLatin1Char('\n'));
    int CDOId = -1;
    Q_FOREACH(const QString &property, properties) {
        if (property.startsWith(QLatin1String("X-MICROSOFT-CDO-TZID"))) {
            QStringList parsed = property.split(QLatin1Char(':'));
            if (parsed.length() != 2) {
                /* Fail */
                return tzid;
            }

            CDOId = parsed.at(1).toInt();
            break;
        }
    }

    /* Wheeee, we have X-MICROSOFT-CDO-TZID, try to map it to Olson format */
    if (CDOId > -1) {

        /* *sigh* Some expert in MS assigned the same ID to two two different timezones... */
        if (CDOId == 2) {

            /* GMT Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
            if (tzid.contains(QLatin1String("Dublin")) ||
                tzid.contains(QLatin1String("Edinburgh")) ||
                tzid.contains(QLatin1String("Lisbon")) ||
                tzid.contains(QLatin1String("London")))
            {
                return QLatin1String("Europe/London");
            }

            /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
            else if (tzid.contains(QLatin1String("Sarajevo")) ||
                     tzid.contains(QLatin1String("Skopje")) ||
                     tzid.contains(QLatin1String("Sofija")) ||
                     tzid.contains(QLatin1String("Vilnius")) ||
                     tzid.contains(QLatin1String("Warsaw")) ||
                     tzid.contains(QLatin1String("Zagreb")))
            {
                return QLatin1String("Europe/Sarajevo");
            }

            /* This should never ever happen. I don't know what to do if it happens */
            else {
                return tzid;
            }
        }

        if (MSCDOTZIDTable.contains(CDOId)) {
            return MSCDOTZIDTable.value(CDOId);
        }
    }

    /* Fail. The event does not have a valid timezone, does not contain
     * X-MICROSOFT-CDO-TZID or we failed to map the CDO-TZID to a real timezone.
     * We are screwed. Just return the original TZID and hope Google will accept it
     * (though we know they won't) */
    return tzid;
}
