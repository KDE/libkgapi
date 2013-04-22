/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "calendarservice.h"
#include "calendar.h"
#include "event.h"
#include "reminder.h"

#include <KCalCore/Alarm>
#include <KCalCore/Event>
#include <KCalCore/Attendee>
#include <KCalCore/Person>
#include <KCalCore/Recurrence>
#include <KCalCore/RecurrenceRule>
#include <KCalCore/ICalFormat>

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <KSystemTimeZones>
#include <KUrl>

#include <QtCore/QVariant>

namespace KGAPI2
{

namespace CalendarService
{

namespace Private
{
    KCalCore::DateList parseRDate(const QString &rule);

    ObjectPtr JSONToCalendar(const QVariantMap &data);
    ObjectPtr JSONToEvent(const QVariantMap &data);

    /**
     * Checks whether TZID is in Olson format and converts it to it if neccessarry
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
    QString checkAndConverCDOTZID(const QString &tzid, const EventPtr& event);

}

/************* URLS **************/

QUrl fetchCalendarsUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/calendar/v3/users/me/calendarList"));
}

QUrl fetchCalendarUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/users/me/calendarList/"));
    url.addPath(calendarID);

    return url;
}

QUrl updateCalendarUrl(const QString &calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);

    return url;
}

QUrl createCalendarUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/calendar/v3/calendars"));
}

QUrl removeCalendarUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);

    return url;
}

QUrl fetchEventsUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));

    return url;
}

QUrl fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl updateEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl createEventUrl(const QString& calendarID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));

    return url;
}

QUrl removeEventUrl(const QString& calendarID, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(calendarID);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);

    return url;
}

QUrl moveEventUrl(const QString& sourceCalendar, const QString& destCalendar, const QString& eventID)
{
    KUrl url(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
    url.addPath(sourceCalendar);
    url.addPath(QLatin1String("events"));
    url.addPath(eventID);
    url.addQueryItem(QLatin1String("destination"), destCalendar);

    return url;
}


QString APIVersion()
{
    return QLatin1String("3");
}

CalendarPtr JSONToCalendar(const QByteArray& jsonData)
{
    QJson::Parser parser;

    QVariantMap calendar = parser.parse(jsonData).toMap();

    if ((calendar.value(QLatin1String("kind")) != QLatin1String("calendar#calendarListEntry")) &&
        (calendar.value(QLatin1String("kind")) != QLatin1String("calendar#calendar"))) {
        return CalendarPtr();
    }

    return Private::JSONToCalendar(calendar).staticCast<Calendar>();
}

ObjectPtr Private::JSONToCalendar(const QVariantMap& data)
{
    CalendarPtr calendar(new Calendar);

    QString id = QUrl::fromPercentEncoding(data.value(QLatin1String("id")).toByteArray());
    calendar->setUid(id);
    calendar->setEtag(data.value(QLatin1String("etag")).toString());
    calendar->setTitle(data.value(QLatin1String("summary")).toString());
    calendar->setDetails(data.value(QLatin1String("description")).toString());
    calendar->setLocation(data.value(QLatin1String("location")).toString());
    calendar->setTimezone(data.value(QLatin1String("timeZone")).toString());

    if ((data.value(QLatin1String("accessRole")).toString() == QLatin1String("writer")) ||
        (data.value(QLatin1String("accessRole")).toString() == QLatin1String("owner"))) {
        calendar->setEditable(true);
    } else {
        calendar->setEditable(false);
    }

    QVariantList reminders = data.value(QLatin1String("defaultReminders")).toList();
    Q_FOREACH(const QVariant & r, reminders) {
        QVariantMap reminder = r.toMap();

        ReminderPtr rem(new Reminder());
        if (reminder.value(QLatin1String("method")).toString() == QLatin1String("email")) {
            rem->setType(KCalCore::Alarm::Email);
        } else if (reminder.value(QLatin1String("method")).toString() == QLatin1String("popup")) {
            rem->setType(KCalCore::Alarm::Display);
        } else {
            rem->setType(KCalCore::Alarm::Invalid);
        }

        rem->setStartOffset(KCalCore::Duration(reminder.value(QLatin1String("minutes")).toInt() * (-60)));

        calendar->addDefaultReminer(rem);
    }

    return calendar.dynamicCast<Object>();
}

QByteArray calendarToJSON(const CalendarPtr& calendar)
{
    QVariantMap output, entry;

    if (!calendar->uid().isEmpty()) {
	entry.insert(QLatin1String("id"), calendar->uid());
    }

    entry.insert(QLatin1String("summary"), calendar->title());
    entry.insert(QLatin1String("description"), calendar->details());
    entry.insert(QLatin1String("location"), calendar->location());
    if (!calendar->timezone().isEmpty()) {
	entry.insert(QLatin1String("timeZone"), calendar->timezone());
    }

    QJson::Serializer serializer;
    return serializer.serialize(entry);
}

ObjectsList parseCalendarJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QVariantMap data = parser.parse(jsonFeed).toMap();

    ObjectsList list;

    if (data.value(QLatin1String("kind")) == QLatin1String("calendar#calendarList")) {
        if (data.contains(QLatin1String("nextPageToken"))) {
            feedData.nextPageUrl = fetchCalendarsUrl();
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), data.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"),QLatin1String("20"));
            }
        }
    } else {
        return ObjectsList();
    }

    const QVariantList items = data.value(QLatin1String("items")).toList();
    Q_FOREACH(const QVariant &i, items) {
        list.append(Private::JSONToCalendar(i.toMap()));
    }

    return list;
}

EventPtr JSONToEvent(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QVariantMap data = parser.parse(jsonData).toMap();

    if (data.value(QLatin1String("kind")) != QLatin1String("calendar#event")) {
        return EventPtr();
    }

    return Private::JSONToEvent(data).staticCast<Event>();
}

ObjectPtr Private::JSONToEvent(const QVariantMap& data)
{
    EventPtr event(new Event);

    /* ID */
    event->setUid(QUrl::fromPercentEncoding(data.value(QLatin1String("id")).toByteArray()));

    /* ETAG */
    event->setEtag(data.value(QLatin1String("etag")).toString());

    /* Status */
    if (data.value(QLatin1String("status")).toString() == QLatin1String("confirmed")) {
        event->setStatus(KCalCore::Incidence::StatusConfirmed);
    } else if (data.value(QLatin1String("status")).toString() == QLatin1String("cancelled")) {
        event->setStatus(KCalCore::Incidence::StatusCanceled);
        event->setDeleted(true);
    } else if (data.value(QLatin1String("status")).toString() == QLatin1String("tentative")) {
        event->setStatus(KCalCore::Incidence::StatusTentative);
    } else {
        event->setStatus(KCalCore::Incidence::StatusNone);
    }

    /* Canceled instance of recurring event. Set ID of the instance to match ID of the event */
    if (data.contains(QLatin1String("recurringEventId"))) {
        event->setUid(QUrl::fromPercentEncoding(data.value(QLatin1String("recurringEventId")).toByteArray()));
    }

    /* Created */
    event->setCreated(KDateTime::fromString(data.value(QLatin1String("created")).toString(), KDateTime::RFC3339Date));

    /* Last updated */
    event->setLastModified(KDateTime::fromString(data.value(QLatin1String("updated")).toString(), KDateTime::RFC3339Date));

    /* Summary */
    event->setSummary(data.value(QLatin1String("summary")).toString());

    /* Description */
    event->setDescription(data.value(QLatin1String("description")).toString());

    /* Location */
    event->setLocation(data.value(QLatin1String("location")).toString());

    /* Start date */
    QVariantMap startData = data.value(QLatin1String("start")).toMap();
    KDateTime dtStart;
    if (startData.contains(QLatin1String("date"))) {
        dtStart = KDateTime::fromString(startData.value(QLatin1String("date")).toString(), KDateTime::ISODate);
        event->setAllDay(true);
    } else if (startData.contains(QLatin1String("dateTime"))) {
        dtStart = KDateTime::fromString(startData.value(QLatin1String("dateTime")).toString(), KDateTime::RFC3339Date);
    }
    /*if (startData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(startData["timeZone"].toString());
        dtStart.setTimeSpec(KDateTime::Spec(tz));
    }*/
    event->setDtStart(dtStart);

    /* End date */
    QVariantMap endData = data.value(QLatin1String("end")).toMap();
    KDateTime dtEnd;
    if (endData.contains(QLatin1String("date"))) {
        dtEnd = KDateTime::fromString(endData.value(QLatin1String("date")).toString(), KDateTime::ISODate);
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        dtEnd = dtEnd.addDays(-1);
        event->setAllDay(true);
    } else if (endData.contains(QLatin1String("dateTime"))) {
        dtEnd = KDateTime::fromString(endData.value(QLatin1String("dateTime")).toString(), KDateTime::RFC3339Date);
    }
    /*if (endData.contains("timeZone")) {
        KTimeZone tz = KSystemTimeZones::zone(endData["timeZone"].toString());
        dtEnd.setTimeSpec(KDateTime::Spec(tz));
    }*/
    event->setDtEnd(dtEnd);

    /* Transparency */
    if (data.value(QLatin1String("transparency")).toString() == QLatin1String("transparent")) {
        event->setTransparency(Event::Transparent);
    } else { /* Assume opaque as default transparency */
        event->setTransparency(Event::Opaque);
    }

    /* Attendees */
    QVariantList attendees = data.value(QLatin1String("attendees")).toList();
    Q_FOREACH(const QVariant & a, attendees) {
        QVariantMap att = a.toMap();
        KCalCore::Attendee::Ptr attendee(
            new KCalCore::Attendee(
                    att.value(QLatin1String("displayName")).toString(),
                    att.value(QLatin1String("email")).toString()));

        if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("accepted"))
            attendee->setStatus(KCalCore::Attendee::Accepted);
        else if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("declined"))
            attendee->setStatus(KCalCore::Attendee::Declined);
        else if (att.value(QLatin1String("responseStatus")).toString() == QLatin1String("tentative"))
            attendee->setStatus(KCalCore::Attendee::Tentative);
        else
            attendee->setStatus(KCalCore::Attendee::NeedsAction);

        if (att.value(QLatin1String("optional")).toBool()) {
            attendee->setRole(KCalCore::Attendee::OptParticipant);
        }

        event->addAttendee(attendee, true);
    }

    /* According to RFC, only events with attendees can have an organizer.
     * Google seems to ignore it, so we must take care of it here */
    if (event->attendeeCount() > 0) {
        KCalCore::Person::Ptr organizer(new KCalCore::Person);
        QVariantMap organizerData = data.value(QLatin1String("organizer")).toMap();
        organizer->setName(organizerData.value(QLatin1String("displayName")).toString());
        organizer->setEmail(organizerData.value(QLatin1String("email")).toString());
        event->setOrganizer(organizer);
    }

    /* Recurrence */
    QStringList recrs = data.value(QLatin1String("recurrence")).toStringList();
    Q_FOREACH(const QString & rec, recrs) {
        KCalCore::ICalFormat format;
        if (rec.left(5) == QLatin1String("RRULE")) {
            KCalCore::RecurrenceRule *recurrenceRule = new KCalCore::RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(6));
            recurrenceRule->setRRule(rec);
            event->recurrence()->addRRule(recurrenceRule);
        } else if (rec.left(6) == QLatin1String("EXRULE")) {
            KCalCore::RecurrenceRule *recurrenceRule = new KCalCore::RecurrenceRule();
            format.fromString(recurrenceRule, rec.mid(7));
            recurrenceRule->setRRule(rec);
            event->recurrence()->addExRule(recurrenceRule);
        } else if (rec.left(6) == QLatin1String("EXDATE")) {
            KCalCore::DateList exdates = Private::parseRDate(rec);
            event->recurrence()->setExDates(exdates);
        } else if (rec.left(5) == QLatin1String("RDATE")) {
            KCalCore::DateList rdates = Private::parseRDate(rec);
            event->recurrence()->setRDates(rdates);
        }
    }

    QVariantMap reminders = data.value(QLatin1String("reminders")).toMap();
    if (reminders.contains(QLatin1String("useDefault")) && reminders.value(QLatin1String("useDefault")).toBool()) {
        event->setUseDefaultReminders(true);
    } else {
        event->setUseDefaultReminders(false);
    }

    QVariantList overrides = reminders.value(QLatin1String("overrides")).toList();
    Q_FOREACH(const QVariant & r, overrides) {
        QVariantMap override = r.toMap();
        KCalCore::Alarm::Ptr alarm(new KCalCore::Alarm(static_cast<KCalCore::Incidence*>(event.data())));
        alarm->setTime(event->dtStart());

        if (override.value(QLatin1String("method")).toString() == QLatin1String("popup")) {
            alarm->setType(KCalCore::Alarm::Display);
        } else if (override.value(QLatin1String("method")).toString() == QLatin1String("email")) {
            alarm->setType(KCalCore::Alarm::Email);
        } else {
            alarm->setType(KCalCore::Alarm::Invalid);
            continue;
        }

        alarm->setStartOffset(KCalCore::Duration(override.value(QLatin1String("minutes")).toInt() * (-60)));
        alarm->setEnabled(true);
        event->addAlarm(alarm);
    }

    /* Extended properties */
    QVariantMap extendedProperties = data.value(QLatin1String("extendedProperties")).toMap();

    QVariantMap privateProperties = extendedProperties.value(QLatin1String("private")).toMap();
    QMap< QString, QVariant >::const_iterator iter = privateProperties.constBegin();
    while (iter != privateProperties.constEnd()) {
        if (iter.key() == QLatin1String("categories")) {
            event->setCategories(iter.value().toString());
        }

        ++iter;
    }

    QVariantMap sharedProperties = extendedProperties.value(QLatin1String("shared")).toMap();
    iter = sharedProperties.constBegin();
    while (iter != sharedProperties.constEnd()) {
        if (iter.key() == QLatin1String("categories")) {
            event->setCategories(iter.value().toString());
        }

        ++iter;
    }

    return event.dynamicCast<Object>();
}

QByteArray eventToJSON(const EventPtr& event)
{
    QVariantMap output, data;

    /* Type */
    data.insert(QLatin1String("type"), QLatin1String("calendar#event"));

    /* ID */
    if (!event->uid().isEmpty()) {
	data.insert(QLatin1String("id"), event->uid());
    }

    /* Status */
    if (event->status() == KCalCore::Incidence::StatusConfirmed) {
        data.insert(QLatin1String("status"), QLatin1String("confirmed"));
    } else if (event->status() == KCalCore::Incidence::StatusCanceled) {
        data.insert(QLatin1String("status"), QLatin1String("canceled"));
    } else if (event->status() == KCalCore::Incidence::StatusTentative) {
        data.insert(QLatin1String("status"), QLatin1String("tentative"));
    }

    /* Summary */
    data.insert(QLatin1String("summary"), event->summary());

    /* Description */
    data.insert(QLatin1String("description"), event->description());

    /* Location */
    data.insert(QLatin1String("location"), event->location());

    /* Recurrence */
    QVariantList recurrence;
    KCalCore::ICalFormat format;
    Q_FOREACH(KCalCore::RecurrenceRule * rRule, event->recurrence()->rRules()) {
        recurrence << format.toString(rRule).remove(QLatin1String("\r\n"));
    }

    Q_FOREACH(KCalCore::RecurrenceRule * rRule, event->recurrence()->exRules()) {
        recurrence << format.toString(rRule).remove(QLatin1String("\r\n"));
    }

    QStringList dates;
    Q_FOREACH(const QDate & rDate, event->recurrence()->rDates()) {
        dates << rDate.toString(QLatin1String("yyyyMMdd"));
    }

    if (!dates.isEmpty()) {
        recurrence << QLatin1String("RDATE;VALUE=DATA:") + dates.join(QLatin1String(","));
    }

    dates.clear();
    Q_FOREACH(const QDate & exDate, event->recurrence()->exDates()) {
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
    if (event->allDay()) {
	start.insert(QLatin1String("date"), event->dtStart().toString(QLatin1String("%Y-%m-%d")));
    } else {
	start.insert(QLatin1String("dateTime"), event->dtStart().toString(KDateTime::RFC3339Date));
    }
    QString tzStart = event->dtStart().timeZone().name();
    if (!recurrence.isEmpty() && tzStart.isEmpty()) {
        tzStart = KTimeZone::utc().name();
    }
    if (!tzStart.isEmpty()) {
	start.insert(QLatin1String("timeZone"), Private::checkAndConverCDOTZID(tzStart, event));
    }
    data.insert(QLatin1String("start"), start);

    /* End */
    QVariantMap end;
    if (event->allDay()) {
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        KDateTime dtEnd = event->dtEnd().addDays(1);
	end.insert(QLatin1String("date"), dtEnd.toString(QLatin1String("%Y-%m-%d")));
    } else {
        end.insert(QLatin1String("dateTime"), event->dtEnd().toString(KDateTime::RFC3339Date));
    }
    QString tzEnd = event->dtEnd().timeZone().name();
    if (!recurrence.isEmpty() && tzEnd.isEmpty()) {
        tzEnd = KTimeZone::utc().name();
    }
    if (!tzEnd.isEmpty()) {
        end.insert(QLatin1String("timeZone"), Private::checkAndConverCDOTZID(tzEnd, event));
    }
    data.insert(QLatin1String("end"), end);

    /* Transparency */
    if (event->transparency() == Event::Transparent) {
        data.insert(QLatin1String("transparency"), QLatin1String("transparent"));
    } else {
        data.insert(QLatin1String("transparency"), QLatin1String("opaque"));
    }

    /* Attendees */
    QVariantList atts;
    Q_FOREACH(const KCalCore::Attendee::Ptr& attee, event->attendees()) {
        QVariantMap att;

	att.insert(QLatin1String("displayName"), attee->name());
	att.insert(QLatin1String("email"), attee->email());

        if (attee->status() == KCalCore::Attendee::Accepted) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("accepted"));
	} else if (attee->status() == KCalCore::Attendee::Declined) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("declined"));
	} else if (attee->status() == KCalCore::Attendee::Tentative) {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("tentative"));
	} else {
	    att.insert(QLatin1String("responseStatus"), QLatin1String("needsAction"));
	}

        if (attee->role() == KCalCore::Attendee::OptParticipant) {
	    att.insert(QLatin1String("optional"), true);
        }

        atts.append(att);
    }

    if (!atts.isEmpty()) {
	data.insert(QLatin1String("attendees"), atts);

        /* According to RFC, event without attendees should not have
         * any organizer. */
        KCalCore::Person::Ptr organizer = event->organizer();
        if (!organizer->isEmpty()) {
            QVariantMap org;
	    org.insert(QLatin1String("displayName"), organizer->fullName());
            org.insert(QLatin1String("email"), organizer->email());
            data.insert(QLatin1String("organizer"), org);
        }
    }

    /* Reminders */
    QVariantList overrides;
    Q_FOREACH(KCalCore::Alarm::Ptr alarm, event->alarms()) {
        QVariantMap override;

        if (alarm->type() == KCalCore::Alarm::Display) {
	    override.insert(QLatin1String("method"), QLatin1String("popup"));
        } else if (alarm->type() == KCalCore::Alarm::Email) {
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
    if (!event->categories().isEmpty()) {
        QVariantMap extendedProperties;
        QVariantMap sharedProperties;
	sharedProperties.insert(QLatin1String("categories"), event->categoriesStr());
	extendedProperties.insert(QLatin1String("shared"), sharedProperties);
	data.insert(QLatin1String("extendedProperties"), extendedProperties);
    }

    /* TODO: Implement support for additional features:
     * http://code.google.com/apis/gdata/docs/2.0/elements.html
     */

    QJson::Serializer serializer;
    return serializer.serialize(data);
}

ObjectsList parseEventJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QVariantMap data = parser.parse(jsonFeed).toMap();

    ObjectsList list;

    if (data.value(QLatin1String("kind")) == QLatin1String("calendar#events")) {
        if (data.contains(QLatin1String("nextPageToken"))) {
            QString calendarId = feedData.requestUrl.toString().remove(QLatin1String("https://www.googleapis.com/calendar/v3/calendars/"));
            calendarId = calendarId.left(calendarId.indexOf(QLatin1Char('/')));
            feedData.nextPageUrl = feedData.requestUrl;
            // replace the old pageToken with a new one
            feedData.nextPageUrl.removeQueryItem(QLatin1String("pageToken"));
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), data.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
            }
        }
    } else {
        return ObjectsList();
    }

    const QVariantList items = data.value(QLatin1String("items")).toList();
    Q_FOREACH(const QVariant &i, items) {
        list.append(Private::JSONToEvent(i.toMap()));
    }

    return list;
}

/******************************** PRIVATE ***************************************/

KCalCore::DateList Private::parseRDate(const QString& rule)
{
    KCalCore::DateList list;
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
            KDateTime kdt = KDateTime::fromString(start, KDateTime::RFC3339Date);
            if (tz.isValid()) {
                kdt.setTimeSpec(tz);
            }

            dt = kdt.date();
        } else {
            KDateTime kdt = KDateTime::fromString(date, KDateTime::RFC3339Date);
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

QString Private::checkAndConverCDOTZID(const QString& tzid, const EventPtr& event)
{
    /* Try to match the @tzid to any valid timezone we know. */
    KTimeZones timeZones;
    const KTimeZones::ZoneMap zones = timeZones.zones();
    KTimeZones::ZoneMap::const_iterator iter;
    for (iter = zones.constBegin(); iter != zones.constEnd(); ++iter) {
        if (iter.key() == tzid) {
            /* Yay, @tzid is a valid TZID in Olson format */
            return tzid;
        }
    }

    /* Damn, no match. Parse the iCal and try to find X-MICROSOFT-CDO-TZID
     * property that we can match against the MSCDOTZIDTable */
    KCalCore::ICalFormat format;
    /* Use a copy of @event, otherwise it would be deleted when ptr is destroyed */
    KCalCore::Incidence::Ptr incidence = event.dynamicCast<KCalCore::Incidence>();
    const QString vcard = format.toICalString(incidence);
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

} // namespace CalendarService

} // namespace KGAPI2
