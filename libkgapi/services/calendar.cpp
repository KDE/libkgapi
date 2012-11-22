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

QUrl Services::Calendar::ScopeUrl(QLatin1String("https://www.googleapis.com/auth/calendar"));

static const QString serviceNameStr(QLatin1String("KGAPI::Services::Calendar"));


const QString& Services::Calendar::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Calendar >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}

/********** PUBLIC JSON INTERFACE ************/

KGAPI::Object* Services::Calendar::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;

    QVariantMap object = parser.parse(jsonData).toMap();

    if ((object.value(QLatin1String("kind")) == QLatin1String("calendar#calendarListEntry")) ||
	(object.value(QLatin1String("kind")) == QLatin1String("calendar#calendar"))) {
        return CalendarPrivate::JSONToCalendar(object);
    } else if (object.value(QLatin1String("kind")) == QLatin1String("calendar#event")) {
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

    if (data.value(QLatin1String("kind")) == QLatin1String("calendar#calendarList")) {
        list = CalendarPrivate::parseCalendarJSONFeed(data.value(QLatin1String("items")).toList());

        if (data.contains(QLatin1String("nextPageToken"))) {
            feedData.nextPageUrl = fetchCalendarsUrl();
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), data.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"),QLatin1String("20"));
            }
        }

    } else if (data.value(QLatin1String("kind")) == QLatin1String("calendar#events")) {
        list = CalendarPrivate::parseEventJSONFeed(data.value(QLatin1String("items")).toList());
        if (data.contains(QLatin1String("nextPageToken"))) {
            QString calendarId = feedData.requestUrl.toString().remove(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
            calendarId = calendarId.left(calendarId.indexOf(QLatin1Char('/')));
            feedData.nextPageUrl = fetchEventsUrl(calendarId);
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), data.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
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
    return QUrl(QLatin1String("https://www.googleapis.com/calendar/v3/users/me/calendarList"));
}

QUrl Services::Calendar::fetchCalendarUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/users/me/calendarList/"));
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::updateCalendarUrl(const QString &calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::createCalendarUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/calendar/v3/calendars"));
}

QUrl Services::Calendar::removeCalendarUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);

    return url;
}

QUrl Services::Calendar::fetchEventsUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));

    return url;
}

QUrl Services::Calendar::fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::updateEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::createEventUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));

    return url;
}

QUrl Services::Calendar::removeEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl Services::Calendar::moveEventUrl(const QString& sourceCalendar, const QString& destCalendar, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(sourceCalendar);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);
    url.addQueryItem(QLatin1String("destination"), destCalendar);

    return url;
}


QString Services::Calendar::protocolVersion() const
{
    return QLatin1String("3");
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

    QString id = QUrl::fromPercentEncoding(calendar.value(QLatin1String("id")).toByteArray());
    object->setUid(id);
    object->setEtag(calendar.value(QLatin1String("etag")).toString());
    object->setTitle(calendar.value(QLatin1String("summary")).toString());
    object->setDetails(calendar.value(QLatin1String("description")).toString());
    object->setLocation(calendar.value(QLatin1String("location")).toString());
    object->setTimezone(calendar.value(QLatin1String("timeZone")).toString());

    if ((calendar.value(QLatin1String("accessRole")).toString() == QLatin1String("writer")) ||
	(calendar.value(QLatin1String("accessRole")).toString() == QLatin1String("owner"))) {
        object->setEditable(true);
    } else {
        object->setEditable(false);
    }

    QVariantList reminders = calendar.value(QLatin1String("defaultReminders")).toList();
    Q_FOREACH(const QVariant & r, reminders) {
        QVariantMap reminder = r.toMap();

        Objects::Reminder::Ptr rem(new Objects::Reminder());
        if (reminder.value(QLatin1String("method")).toString() == QLatin1String("email")) {
            rem->setType(Alarm::Email);
        } else if (reminder.value(QLatin1String("method")).toString() == QLatin1String("popup")) {
            rem->setType(Alarm::Display);
        } else {
            rem->setType(Alarm::Invalid);
        }

        rem->setStartOffset(Duration(reminder.value(QLatin1String("minutes")).toInt() * (-60)));

        object->addDefaultReminer(rem);
    }

    return dynamic_cast< KGAPI::Object* >(object);
}

QVariantMap Services::CalendarPrivate::calendarToJSON(KGAPI::Object* calendar)
{
    QVariantMap output, entry;
    Objects::Calendar *object = static_cast< Objects::Calendar* >(calendar);

    if (!object->uid().isEmpty()) {
	entry.insert(QLatin1String("id"), object->uid());
    }

    entry.insert(QLatin1String("summary"), object->title());
    entry.insert(QLatin1String("description"), object->details());
    entry.insert(QLatin1String("location"), object->location());
    if (!object->timezone().isEmpty()) {
	entry.insert(QLatin1String("timeZone"), object->timezone());
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
    object->setUid(QUrl::fromPercentEncoding(event.value(QLatin1String("id")).toByteArray()));

    /* ETAG */
    object->setEtag(event.value(QLatin1String("etag")).toString());

    /* Status */
    if (event.value(QLatin1String("status")).toString() == QLatin1String("confirmed")) {
        object->setStatus(Incidence::StatusConfirmed);
    } else if (event.value(QLatin1String("status")).toString() == QLatin1String("cancelled")) {
        object->setStatus(Incidence::StatusCanceled);
        object->setDeleted(true);
    } else if (event.value(QLatin1String("status")).toString() == QLatin1String("tentative")) {
        object->setStatus(Incidence::StatusTentative);
    } else {
        object->setStatus(Incidence::StatusNone);
    }

    /* Canceled instance of recurring event. Set ID of the instance to match ID of the event */
    if (event.contains(QLatin1String("recurringEventId"))) {
        object->setUid(QUrl::fromPercentEncoding(event.value(QLatin1String("recurringEventId")).toByteArray()));
    }

    /* Created */
    object->setCreated(AccessManager::RFC3339StringToDate(event.value(QLatin1String("created")).toString()));

    /* Last updated */
    object->setLastModified(AccessManager::RFC3339StringToDate(event.value(QLatin1String("updated")).toString()));

    /* Summary */
    object->setSummary(event.value(QLatin1String("summary")).toString());

    /* Description */
    object->setDescription(event.value(QLatin1String("description")).toString());

    /* Location */
    object->setLocation(event.value(QLatin1String("location")).toString());

    /* Start date */
    QVariantMap startData = event.value(QLatin1String("start")).toMap();
    KDateTime dtStart;
    if (startData.contains(QLatin1String("date"))) {
        dtStart = KDateTime::fromString(startData.value(QLatin1String("date")).toString(), KDateTime::ISODate);
        object->setAllDay(true);
    } else if (startData.contains(QLatin1String("dateTime"))) {
        dtStart = AccessManager::RFC3339StringToDate(startData.value(QLatin1String("dateTime")).toString());
    }
    /*if (startData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(startData["timeZone"].toString());
        dtStart.setTimeSpec(KDateTime::Spec(tz));
    }*/
    object->setDtStart(dtStart);

    /* End date */
    QVariantMap endData = event.value(QLatin1String("end")).toMap();
    KDateTime dtEnd;
    if (endData.contains(QLatin1String("date"))) {
        dtEnd = KDateTime::fromString(endData.value(QLatin1String("date")).toString(), KDateTime::ISODate);
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        dtEnd = dtEnd.addDays(-1);
        object->setAllDay(true);
    } else if (endData.contains(QLatin1String("dateTime"))) {
        dtEnd = AccessManager::RFC3339StringToDate(endData.value(QLatin1String("dateTime")).toString());
    }
    /*if (endData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(endData["timeZone"].toString());
        dtEnd.setTimeSpec(KDateTime::Spec(tz));
    }*/
    object->setDtEnd(dtEnd);

    /* Transparency */
    if (event.value(QLatin1String("transparency")).toString() == QLatin1String("transparent")) {
        object->setTransparency(Event::Transparent);
    } else { /* Assume opaque as default transparency */
        object->setTransparency(Event::Opaque);
    }

    /* Attendees */
    QVariantList attendees = event.value(QLatin1String("attendees")).toList();
    Q_FOREACH(const QVariant & a, attendees) {
        QVariantMap att = a.toMap();
        AttendeePtr attendee(
            new Attendee(att.value(QLatin1String("displayName")).toString(),
                         att.value(QLatin1String("email")).toString()));

        if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("accepted"))
            attendee->setStatus(Attendee::Accepted);
        else if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("declined"))
            attendee->setStatus(Attendee::Declined);
        else if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("tentative"))
            attendee->setStatus(Attendee::Tentative);
        else
            attendee->setStatus(Attendee::NeedsAction);

        if (att.value(QLatin1String("optional")).toBool()) {
            attendee->setRole(Attendee::OptParticipant);
        }

        object->addAttendee(attendee, true);
    }

    /* According to RFC, only events with attendees can have an organizer.
     * Google seems to ignore it, so we must take care of it here */
    if (object->attendeeCount() > 0) {
        PersonPtr organizer(new Person);
        QVariantMap organizerData = event.value(QLatin1String("organizer")).toMap();
        organizer->setName(organizerData.value(QLatin1String("displayName")).toString());
        organizer->setEmail(organizerData.value(QLatin1String("email")).toString());
    #ifdef WITH_KCAL
        object->setOrganizer(*organizer);
    #else
        object->setOrganizer(organizer);
    #endif
    }

    /* Recurrence */
    QStringList recrs = event.value(QLatin1String("recurrence")).toStringList();
    Q_FOREACH(const QString & rec, recrs) {
        ICalFormat format;
        if (rec.left(5) == QLatin1String("RRULE")) {
            RecurrenceRule *recurrenceRule = new RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(6));
            recurrenceRule->setRRule(rec);
            object->recurrence()->addRRule(recurrenceRule);
        } else if (rec.left(6) == QLatin1String("EXRULE")) {
            RecurrenceRule *recurrenceRule = new RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(7));
            recurrenceRule->setRRule(rec);
            object->recurrence()->addExRule(recurrenceRule);
        } else if (rec.left(6) == QLatin1String("EXDATE")) {
            DateList exdates = parseRDate(rec);
            object->recurrence()->setExDates(exdates);
        } else if (rec.left(5) == QLatin1String("RDATE")) {
            DateList rdates = parseRDate(rec);
            object->recurrence()->setRDates(rdates);
        }
    }

    QVariantMap reminders = event.value(QLatin1String("reminders")).toMap();
    if (reminders.contains(QLatin1String("useDefault")) && reminders.value(QLatin1String("useDefault")).toBool()) {
        object->setUseDefaultReminders(true);
    } else {
        object->setUseDefaultReminders(false);
    }

    QVariantList overrides = reminders.value(QLatin1String("overrides")).toList();
    Q_FOREACH(const QVariant & r, overrides) {
        QVariantMap override = r.toMap();
        AlarmPtr alarm(new Alarm(object));
        alarm->setTime(object->dtStart());

        if (override.value(QLatin1String("method")).toString() == QLatin1String("popup")) {
            alarm->setType(Alarm::Display);
        } else if (override.value(QLatin1String("method")).toString() == QLatin1String("email")) {
            alarm->setType(Alarm::Email);
        } else {
            alarm->setType(Alarm::Invalid);
            continue;
        }

        alarm->setStartOffset(Duration(override.value(QLatin1String("minutes")).toInt() * (-60)));
        alarm->setEnabled(true);
        object->addAlarm(alarm);
    }

    /* Extended properties */
    QVariantMap extendedProperties = event.value(QLatin1String("extendedProperties")).toMap();

    QVariantMap privateProperties = extendedProperties.value(QLatin1String("private")).toMap();
    QMap< QString, QVariant >::const_iterator iter = privateProperties.constBegin();
    while (iter != privateProperties.constEnd()) {
        if (iter.key() == QLatin1String("categories")) {
            object->setCategories(iter.value().toString());
        }

        ++iter;
    }

    QVariantMap sharedProperties = extendedProperties.value(QLatin1String("shared")).toMap();
    iter = sharedProperties.constBegin();
    while (iter != sharedProperties.constEnd()) {
        if (iter.key() == QLatin1String("categories")) {
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
    data.insert(QLatin1String("type"), QLatin1String("calendar#event"));

    /* ID */
    if (!object->uid().isEmpty()) {
	data.insert(QLatin1String("id"), object->uid());
    }

    /* Status */
    if (object->status() == Incidence::StatusConfirmed) {
        data.insert(QLatin1String("status"), QLatin1String("confirmed"));
    } else if (object->status() == Incidence::StatusCanceled) {
        data.insert(QLatin1String("status"), QLatin1String("canceled"));
    } else if (object->status() == Incidence::StatusTentative) {
        data.insert(QLatin1String("status"), QLatin1String("tentative"));
    }

    /* Summary */
    data.insert(QLatin1String("summary"), object->summary());

    /* Description */
    data.insert(QLatin1String("description"), object->description());

    /* Location */
    data.insert(QLatin1String("location"), object->location());

    /* Recurrence */
    QVariantList recurrence;
    ICalFormat format;
    Q_FOREACH(RecurrenceRule * rRule, object->recurrence()->rRules()) {
        recurrence << format.toString(rRule).remove(QLatin1String("\r\n"));
    }

    Q_FOREACH(RecurrenceRule * rRule, object->recurrence()->exRules()) {
        recurrence << format.toString(rRule).remove(QLatin1String("\r\n"));
    }

    QStringList dates;
    Q_FOREACH(const QDate & rDate, object->recurrence()->rDates()) {
        dates << rDate.toString(QLatin1String("yyyyMMdd"));
    }

    if (!dates.isEmpty()) {
        recurrence << QLatin1String("RDATE;VALUE=DATA:") + dates.join(QLatin1String(","));
    }

    dates.clear();
    Q_FOREACH(const QDate & exDate, object->recurrence()->exDates()) {
        dates << exDate.toString(QLatin1String("yyyyMMdd"));
    }

    if (!dates.isEmpty()) {
        recurrence << QLatin1String("EXDATE;VALUE=DATE:") + dates.join(QLatin1String(","));
    }

    if (!recurrence.isEmpty()) {
        data.insert(QLatin1String("recurrence"), recurrence);
    }

    /* Start */
    QVariantMap start;
    if (object->allDay()) {
	start.insert(QLatin1String("date"), object->dtStart().toString(QLatin1String("%Y-%m-%d")));
    } else {
	start.insert(QLatin1String("dateTime"), AccessManager::dateToRFC3339String(object->dtStart()));
    }
    QString tzStart = object->dtStart().timeZone().name();
    if (!recurrence.isEmpty() && tzStart.isEmpty()) {
        tzStart = KTimeZone::utc().name();
    }
    if (!tzStart.isEmpty()) {
	start.insert(QLatin1String("timeZone"), tzStart);
    }
    data.insert(QLatin1String("start"), start);

    /* End */
    QVariantMap end;
    if (object->allDay()) {
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        KDateTime dtEnd = object->dtEnd().addDays(1);
	end.insert(QLatin1String("date"), dtEnd.toString(QLatin1String("%Y-%m-%d")));
    } else {
        end.insert(QLatin1String("dateTime"), AccessManager::dateToRFC3339String(object->dtEnd()));
    }
    QString tzEnd = object->dtEnd().timeZone().name();
    if (!recurrence.isEmpty() && tzEnd.isEmpty()) {
        tzEnd = KTimeZone::utc().name();
    }
    if (!tzEnd.isEmpty()) {
        end.insert(QLatin1String("timeZone"), tzEnd);
    }
    data.insert(QLatin1String("end"), end);

    /* Transparency */
    if (object->transparency() == Event::Transparent) {
        data.insert(QLatin1String("transparency"), QLatin1String("transparent"));
    } else {
        data.insert(QLatin1String("transparency"), QLatin1String("opaque"));
    }

    /* Attendees */
    QVariantList atts;
    Q_FOREACH(const AttendeePtr attee, object->attendees()) {
        QVariantMap att;

	att.insert(QLatin1String("displayName"), attee->name());
	att.insert(QLatin1String("emali"), attee->email());

        if (attee->status() == Attendee::Accepted) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("accepted"));
	} else if (attee->status() == Attendee::Declined) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("declined"));
	} else if (attee->status() == Attendee::Tentative) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("tentative"));
	} else {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("needsAction"));
	}

        if (attee->role() == Attendee::OptParticipant) {
	    att.insert(QLatin1String("optional"), true);
        }

        atts.append(att);
    }

    if (!atts.isEmpty()) {
	data.insert(QLatin1String("attendees"), atts);

        /* According to RFC, event without attendees should not have
         * any organizer. */
    #ifdef WITH_KCAL
        PersonPtr organizer = new Person(object->organizer());
    #else
        PersonPtr organizer = object->organizer();
    #endif
        if (!organizer->isEmpty()) {
            QVariantMap org;
	    org.insert(QLatin1String("displayName"), organizer->fullName());
            org.insert(QLatin1String("email"), organizer->email());
            data.insert(QLatin1String("organizer"), org);
        }
    }

    /* Reminders */
    QVariantList overrides;
    Q_FOREACH(AlarmPtr alarm, object->alarms()) {
        QVariantMap override;

        if (alarm->type() == Alarm::Display) {
	    override.insert(QLatin1String("method"), QLatin1String("popup"));
        } else if (alarm->type() == Alarm::Email) {
	    override.insert(QLatin1String("method"), QLatin1String("email"));
        } else {
            continue;
        }

        override.insert(QLatin1String("minutes"),  (int)(alarm->startOffset().asSeconds() / -60));

        overrides << override;
    }

    QVariantMap reminders;
    reminders.insert(QLatin1String("useDefault"), false);
    reminders.insert(QLatin1String("overrides"), overrides);
    data.insert(QLatin1String("reminders"), reminders);

    /* Store categories */
    if (!object->categories().isEmpty()) {
        QVariantMap extendedProperties;
        QVariantMap sharedProperties;
	sharedProperties.insert(QLatin1String("categories"), object->categoriesStr());
	extendedProperties.insert(QLatin1String("shared"), sharedProperties);
	data.insert(QLatin1String("extendedProperties"), extendedProperties);
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

    QString left = rule.left(rule.indexOf(QLatin1Char(':')));
    QStringList params = left.split(QLatin1Char(';'));
    Q_FOREACH(const QString & param, params) {
        if (param.startsWith(QLatin1String("VALUE"))) {
            value = param.mid(param.indexOf(QLatin1Char('=')) + 1);
        } else if (param.startsWith(QLatin1String("TZID"))) {
            QString tzname = param.mid(param.indexOf(QLatin1Char('=')) + 1);
            tz = KSystemTimeZones::zone(tzname);
        }
    }

    QString datesStr = rule.mid(rule.lastIndexOf(QLatin1Char(':')) + 1);
    QStringList dates = datesStr.split(QLatin1Char(','));
    Q_FOREACH(const QString &date, dates) {
        QDate dt;

        if (value == QLatin1String("DATE")) {
            dt = QDate::fromString(date, QLatin1String("yyyyMMdd"));
        } else if (value == QLatin1String("PERIOD")) {
            QString start = date.left(date.indexOf(QLatin1Char('/')));
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
