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
#include "../debug.h"

#include <KCalCore/Alarm>
#include <KCalCore/Event>
#include <KCalCore/Attendee>
#include <KCalCore/Person>
#include <KCalCore/Recurrence>
#include <KCalCore/RecurrenceRule>
#include <KCalCore/ICalFormat>

#include <QJsonDocument>

#include <KDateTime>
#include <KSystemTimeZones>

#include <QtCore/QVariant>

namespace KGAPI2
{

namespace CalendarService
{

namespace Private
{
    KCalCore::DateList parseRDate(const QString &rule);

    ObjectPtr JSONToCalendar(const QVariantMap &data);
    ObjectPtr JSONToEvent(const QVariantMap &data, const QString &timezone = QString());

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

    static const QUrl GoogleApisUrl(QLatin1String("https://www.googleapis.com"));
    static const QString CalendarListBasePath(QLatin1String("/calendar/v3/users/me/calendarList"));
    static const QString CalendarBasePath(QLatin1String("/calendar/v3/calendars"));
}

/************* URLS **************/

QUrl fetchCalendarsUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarListBasePath);
    return url;
}

QUrl fetchCalendarUrl(const QString& calendarID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarListBasePath % QLatin1Char('/') % calendarID);
    return url;
}

QUrl updateCalendarUrl(const QString &calendarID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarListBasePath % QLatin1Char('/') % calendarID);
    return url;
}

QUrl createCalendarUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath);
    return url;
}

QUrl removeCalendarUrl(const QString& calendarID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID);
    return url;
}

QUrl fetchEventsUrl(const QString& calendarID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events"));
    url.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
    return url;
}

QUrl fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events/") % eventID);
    return url;
}

QUrl updateEventUrl(const QString& calendarID, const QString& eventID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events/") % eventID);
    return url;
}

QUrl createEventUrl(const QString& calendarID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events"));
    return url;
}

QUrl removeEventUrl(const QString& calendarID, const QString& eventID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events/") % eventID);
    return url;
}

QUrl moveEventUrl(const QString& sourceCalendar, const QString& destCalendar, const QString& eventID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % sourceCalendar % QLatin1String("/events/") % eventID);
    url.addQueryItem(QLatin1String("destination"), destCalendar);
    return url;
}


QString APIVersion()
{
    return QLatin1String("3");
}

CalendarPtr JSONToCalendar(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QVariantMap calendar = document.toVariant().toMap();

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
    calendar->setBackgroundColor(QColor(data.value(QLatin1String("backgroundColor")).toString()));
    calendar->setForegroundColor(QColor(data.value(QLatin1String("foregroundColor")).toString()));

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

    QJsonDocument document = QJsonDocument::fromVariant(entry);
    return document.toJson(QJsonDocument::Compact);
}

ObjectsList parseCalendarJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonFeed);
    QVariantMap data = document.toVariant().toMap();

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
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QVariantMap data = document.toVariant().toMap();

    if (data.value(QLatin1String("kind")) != QLatin1String("calendar#event")) {
        return EventPtr();
    }

    return Private::JSONToEvent(data).staticCast<Event>();
}

ObjectPtr Private::JSONToEvent(const QVariantMap& data, const QString &timezone)
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
        // If there's a timezone specified in the "start" entity, then use it
        if (startData.contains(QLatin1String("timeZone"))) {
            const KTimeZone tz = KSystemTimeZones::zone(startData.value(QLatin1String("timeZone")).toString());
            if (tz.isValid()) {
                dtStart = dtStart.toTimeSpec(KDateTime::Spec(tz));
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << startData.value(QLatin1String("timeZone")).toString();
            }

        // Otherwise try to fallback to calendar-wide timezone
        } else if (!timezone.isEmpty()) {
            const KTimeZone tz = KSystemTimeZones::zone(timezone);
            if (tz.isValid()) {
                dtStart = dtStart.toTimeSpec(KDateTime::Spec(tz));
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << timezone;
            }
        }
    }
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
        if (endData.contains(QLatin1String("timeZone"))) {
            const KTimeZone tz = KSystemTimeZones::zone(endData.value(QLatin1String("timeZone")).toString());
            if (tz.isValid()) {
                dtEnd = dtEnd.toTimeSpec(KDateTime::Spec(tz));
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << endData.value(QLatin1String("timeZone")).toString();
            }
        } else if (!timezone.isEmpty()) {
            const KTimeZone tz = KSystemTimeZones::zone(timezone);
            if (tz.isValid()) {
                dtEnd = dtEnd.toTimeSpec(KDateTime::Spec(tz));
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << timezone;
            }
        }
    }
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
    QVariantMap data;

    /* Type */
    data.insert(QStringLiteral("type"), QStringLiteral("calendar#event"));

    /* ID */
    if (!event->uid().isEmpty()) {
    data.insert(QStringLiteral("id"), event->uid());
    }

    /* Status */
    if (event->status() == KCalCore::Incidence::StatusConfirmed) {
        data.insert(QStringLiteral("status"), QStringLiteral("confirmed"));
    } else if (event->status() == KCalCore::Incidence::StatusCanceled) {
        data.insert(QStringLiteral("status"), QStringLiteral("canceled"));
    } else if (event->status() == KCalCore::Incidence::StatusTentative) {
        data.insert(QStringLiteral("status"), QStringLiteral("tentative"));
    }

    /* Summary */
    data.insert(QStringLiteral("summary"), event->summary());

    /* Description */
    data.insert(QStringLiteral("description"), event->description());

    /* Location */
    data.insert(QStringLiteral("location"), event->location());

    /* Recurrence */
    QVariantList recurrence;
    KCalCore::ICalFormat format;
    Q_FOREACH(KCalCore::RecurrenceRule * rRule, event->recurrence()->rRules()) {
        recurrence << format.toString(rRule).remove(QStringLiteral("\r\n"));
    }

    Q_FOREACH(KCalCore::RecurrenceRule * rRule, event->recurrence()->exRules()) {
        recurrence << format.toString(rRule).remove(QStringLiteral("\r\n"));
    }

    QStringList dates;
    Q_FOREACH(const QDate & rDate, event->recurrence()->rDates()) {
        dates << rDate.toString(QStringLiteral("yyyyMMdd"));
    }

    if (!dates.isEmpty()) {
        recurrence << QString(QStringLiteral("RDATE;VALUE=DATA:") + dates.join(QStringLiteral(",")));
    }

    dates.clear();
    Q_FOREACH(const QDate & exDate, event->recurrence()->exDates()) {
        dates << exDate.toString(QStringLiteral("yyyyMMdd"));
    }

    if (!dates.isEmpty()) {
        recurrence << QString(QStringLiteral("EXDATE;VALUE=DATE:") + dates.join(QStringLiteral(",")));
    }

    if (!recurrence.isEmpty()) {
        data.insert(QStringLiteral("recurrence"), recurrence);
    }

    /* Start */
    QVariantMap start;
    if (event->allDay()) {
    start.insert(QStringLiteral("date"), event->dtStart().toString(QStringLiteral("%Y-%m-%d")));
    } else {
    start.insert(QStringLiteral("dateTime"), event->dtStart().toString(KDateTime::RFC3339Date));
    }
    QString tzStart = event->dtStart().timeZone().name();
    if (!recurrence.isEmpty() && tzStart.isEmpty()) {
        tzStart = KTimeZone::utc().name();
    }
    if (!tzStart.isEmpty()) {
    start.insert(QStringLiteral("timeZone"), Private::checkAndConverCDOTZID(tzStart, event));
    }
    data.insert(QStringLiteral("start"), start);

    /* End */
    QVariantMap end;
    if (event->allDay()) {
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        KDateTime dtEnd = event->dtEnd().addDays(1);
    end.insert(QStringLiteral("date"), dtEnd.toString(QStringLiteral("%Y-%m-%d")));
    } else {
        end.insert(QStringLiteral("dateTime"), event->dtEnd().toString(KDateTime::RFC3339Date));
    }
    QString tzEnd = event->dtEnd().timeZone().name();
    if (!recurrence.isEmpty() && tzEnd.isEmpty()) {
        tzEnd = KTimeZone::utc().name();
    }
    if (!tzEnd.isEmpty()) {
        end.insert(QStringLiteral("timeZone"), Private::checkAndConverCDOTZID(tzEnd, event));
    }
    data.insert(QStringLiteral("end"), end);

    /* Transparency */
    if (event->transparency() == Event::Transparent) {
        data.insert(QStringLiteral("transparency"), QStringLiteral("transparent"));
    } else {
        data.insert(QStringLiteral("transparency"), QStringLiteral("opaque"));
    }

    /* Attendees */
    QVariantList atts;
    Q_FOREACH(const KCalCore::Attendee::Ptr& attee, event->attendees()) {
        QVariantMap att;

    att.insert(QStringLiteral("displayName"), attee->name());
    att.insert(QStringLiteral("email"), attee->email());

        if (attee->status() == KCalCore::Attendee::Accepted) {
        att.insert(QStringLiteral("responseStatus"), QStringLiteral("accepted"));
	} else if (attee->status() == KCalCore::Attendee::Declined) {
        att.insert(QStringLiteral("responseStatus"), QStringLiteral("declined"));
	} else if (attee->status() == KCalCore::Attendee::Tentative) {
        att.insert(QStringLiteral("responseStatus"), QStringLiteral("tentative"));
	} else {
        att.insert(QStringLiteral("responseStatus"), QStringLiteral("needsAction"));
	}

        if (attee->role() == KCalCore::Attendee::OptParticipant) {
        att.insert(QStringLiteral("optional"), true);
        }

        atts.append(att);
    }

    if (!atts.isEmpty()) {
    data.insert(QStringLiteral("attendees"), atts);

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
    Q_FOREACH(const KCalCore::Alarm::Ptr &alarm, event->alarms()) {
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

    QJsonDocument document = QJsonDocument::fromVariant(data);
    return document.toJson(QJsonDocument::Compact);
}

ObjectsList parseEventJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonFeed);
    QVariantMap data = document.toVariant().toMap();

    ObjectsList list;

    QString timezone;
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
        if (data.contains(QLatin1String("timeZone"))) {
            // This should always be in Olson format
            timezone = data.value(QLatin1String("timeZone")).toString();
        }
    } else {
        return ObjectsList();
    }

    const QVariantList items = data.value(QLatin1String("items")).toList();
    Q_FOREACH(const QVariant &i, items) {
        list.append(Private::JSONToEvent(i.toMap(), timezone));
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
    map.insert(0,  QStringLiteral("UTC"));
    map.insert(1,  QStringLiteral("Europe/London"));                       /* GMT Greenwich Mean Time; Dublin, Edinburgh, London */
    /* Seriously? *sigh* Let's handle these two in checkAndConvertCDOTZID() */
    //map.insertMulti(2, QStringLiteral("Europe/Lisbon"));         /* GMT Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    //map.insertMulti(2, QStringLiteral("Europe/Sarajevo"));       /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
    map.insert(3,  QStringLiteral("Europe/Paris"));              /* GMT+01:00 Paris, Madrid, Brussels, Copenhagen */
    map.insert(4,  QStringLiteral("Europe/Berlin"));             /* GMT+01:00 Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    map.insert(5,  QStringLiteral("Europe/Bucharest"));          /* GMT+02:00 Bucharest */
    map.insert(6,  QStringLiteral("Europe/Prague"));             /* GMT+01:00 Prague, Central Europe */
    map.insert(7,  QStringLiteral("Europe/Athens"));             /* GMT+02:00 Athens, Istanbul, Minsk */
    map.insert(8,  QStringLiteral("America/Brazil"));            /* GMT-03:00 Brasilia */
    map.insert(9,  QStringLiteral("America/Halifax"));           /* GMT-04:00 Atlantic time (Canada) */
    map.insert(10, QStringLiteral("America/New_York"));          /* GMT-05:00 Eastern Time (US & Canada) */
    map.insert(11, QStringLiteral("America/Chicago"));           /* GMT-06:00 Central Time (US & Canada) */
    map.insert(12, QStringLiteral("America/Denver"));            /* GMT-07:00 Mountain Time (US & Canada) */
    map.insert(13, QStringLiteral("America/Los_Angeles"));       /* GMT-08:00 Pacific Time (US & Canada); Tijuana */
    map.insert(14, QStringLiteral("America/Anchorage"));         /* GMT-09:00 Alaska */
    map.insert(15, QStringLiteral("Pacific/Honolulu"));          /* GMT-10:00 Hawaii */
    map.insert(16, QStringLiteral("Pacific/Apia"));              /* GMT-11:00 Midway Island, Samoa */
    map.insert(17, QStringLiteral("Pacific/Auckland"));          /* GMT+12:00 Auckland, Wellington */
    map.insert(18, QStringLiteral("Australia/Brisbane"));        /* GMT+10:00 Brisbane, East Australia */
    map.insert(19, QStringLiteral("Australia/Adelaide"));        /* GMT+09:30 Adelaide, Central Australia */
    map.insert(20, QStringLiteral("Asia/Tokyo"));                /* GMT+09:00 Osaka, Sapporo, Tokyo */
    map.insert(21, QStringLiteral("Asia/Singapore"));            /* GMT+08:00 Kuala Lumpur, Singapore */
    map.insert(22, QStringLiteral("Asia/Bangkok"));              /* GMT+07:00 Bangkok, Hanoi, Jakarta */
    map.insert(23, QStringLiteral("Asia/Calcutta"));             /* GMT+05:30 Kolkata, Chennai, Mumbai, New Delhi, India Standard Time */
    map.insert(24, QStringLiteral("Asia/Dubai"));                /* GMT+04:00 Abu Dhabi, Muscat */
    map.insert(25, QStringLiteral("Asia/Tehran"));               /* GMT+03:30 Tehran */
    map.insert(26, QStringLiteral("Asia/Baghdad"));              /* GMT+03:00 Baghdad */
    map.insert(27, QStringLiteral("Asia/Jerusalem"));            /* GMT+02:00 Israel, Jerusalem Standard Time */
    map.insert(28, QStringLiteral("America/St_Johns"));          /* GMT-03:30 Newfoundland */
    map.insert(29, QStringLiteral("Atlantic/Portugal"));         /* GMT-01:00 Azores */
    map.insert(30, QStringLiteral("America/Noronha"));           /* GMT-02:00 Mid-Atlantic */
    map.insert(31, QStringLiteral("Africa/Monrovia"));           /* GMT Casablanca, Monrovia */
    map.insert(32, QStringLiteral("America/Argentina/Buenos_Aires")); /* GMT-03:00 Buenos Aires, Georgetown */
    map.insert(33, QStringLiteral("America/La_Paz"));            /* GMT-04:00 Caracas, La Paz */
    map.insert(34, QStringLiteral("America/New_York"));          /* GMT-05:00 Indiana (East) */
    map.insert(35, QStringLiteral("America/Bogota"));            /* GMT-05:00 Bogota, Lima, Quito */
    map.insert(36, QStringLiteral("America/Winnipeg"));          /* GMT-06:00 Saskatchewan */
    map.insert(37, QStringLiteral("America/Mexico_City"));       /* GMT-06:00 Mexico City, Tegucigalpa */
    map.insert(38, QStringLiteral("America/Phoenix"));           /* GMT-07:00 Arizona */
    map.insert(39, QStringLiteral("Pacific/Kwajalein"));         /* GMT-12:00 Eniwetok, Kwajalein, Dateline Time */
    map.insert(40, QStringLiteral("Pacific/Fiji"));              /* GMT+12:00 Fušál, Kamchatka, Mashall Is. */
    map.insert(41, QStringLiteral("Pacific/Noumea"));            /* GMT+11:00 Magadan, Solomon Is., New Caledonia */
    map.insert(42, QStringLiteral("Australia/Hobart"));          /* GMT+10:00 Hobart, Tasmania */
    map.insert(43, QStringLiteral("Pacific/Guam"));              /* GMT+10:00 Guam, Port Moresby */
    map.insert(44, QStringLiteral("Australia/Darwin"));          /* GMT+09:30 Darwin */
    map.insert(45, QStringLiteral("Asia/Shanghai"));             /* GMT+08:00 Beijing, Chongqing, Hong Kong SAR, Urumqi */
    map.insert(46, QStringLiteral("Asia/Omsk"));                 /* GMT+06:00 Almaty, Novosibirsk, North Central Asia */
    map.insert(47, QStringLiteral("Asia/Karachi"));              /* GMT+05:00 Islamabad, Karachi, Tashkent */
    map.insert(48, QStringLiteral("Asia/Kabul"));                /* GMT+04:30 Kabul */
    map.insert(49, QStringLiteral("Africa/Cairo"));              /* GMT+02:00 Cairo */
    map.insert(50, QStringLiteral("Africa/Harare"));             /* GMT+02:00 Harare, Pretoria */
    map.insert(51, QStringLiteral("Europe/Moscow"));             /* GMT+03:00 Moscow, St. Petersburg, Volgograd */
    map.insert(53, QStringLiteral("Atlantic/Cape_Verde"));       /* GMT-01:00 Cape Verde Is. */
    map.insert(54, QStringLiteral("Asia/Tbilisi"));              /* GMT+04:00 Baku, Tbilisi, Yerevan */
    map.insert(55, QStringLiteral("America/Tegucigalpa"));       /* GMT-06:00 Central America */
    map.insert(56, QStringLiteral("Africa/Nairobi"));            /* GMT+03:00 East Africa, Nairobi */
    map.insert(58, QStringLiteral("Asia/Yekaterinburg"));        /* GMT+05:00 Ekaterinburg */
    map.insert(59, QStringLiteral("Europe/Helsinki"));           /* GMT+02:00 Helsinki, Riga, Tallinn */
    map.insert(60, QStringLiteral("America/Greenland"));         /* GMT-03:00 Greenland */
    map.insert(61, QStringLiteral("Asia/Rangoon"));              /* GMT+06:30 Yangon (Rangoon) */
    map.insert(62, QStringLiteral("Asia/Katmandu"));             /* GMT+05:45 Kathmandu, Nepal */
    map.insert(63, QStringLiteral("Asia/Irkutsk"));              /* GMT+08:00 Irkutsk, Ulaan Bataar */
    map.insert(64, QStringLiteral("Asia/Krasnoyarsk"));          /* GMT+07:00 Krasnoyarsk */
    map.insert(65, QStringLiteral("America/Santiago"));          /* GMT-04:00 Santiago */
    map.insert(66, QStringLiteral("Asia/Colombo"));              /* GMT+06:00 Sri Jayawardenepura, Sri Lanka */
    map.insert(67, QStringLiteral("Pacific/Tongatapu"));         /* GMT+13:00 Nuku'alofa, Tonga */
    map.insert(68, QStringLiteral("Asia/Vladivostok"));          /* GMT+10:00 Vladivostok */
    map.insert(69, QStringLiteral("Africa/Bangui"));             /* GMT+01:00 West Central Africa */
    map.insert(70, QStringLiteral("Asia/Yakutsk"));              /* GMT+09:00 Yakutsk */
    map.insert(71, QStringLiteral("Asia/Dhaka"));                /* GMT+06:00 Astana, Dhaka */
    map.insert(72, QStringLiteral("Asia/Seoul"));                /* GMT+09:00 Seoul, Korea Standard time */
    map.insert(73, QStringLiteral("Australia/Perth"));           /* GMT+08:00 Perth, Western Australia */
    map.insert(74, QStringLiteral("Asia/Kuwait"));               /* GMT+03:00 Arab, Kuwait, Riyadh */
    map.insert(75, QStringLiteral("Asia/Taipei"));               /* GMT+08:00 Taipei */
    map.insert(76, QStringLiteral("Australia/Sydney"));          /* GMT+10:00 Canberra, Melbourne, Sydney */

    return map;
}

static QMap<QString, QString> initMSStandardTimeTZTable()
{
    QMap<QString, QString> map;


    /* Based on "Microsoft Time Zone Index Values"
     * http://support.microsoft.com/kb/973627
     *
     * The mapping is not exact, since the the TZID usually refers to a
     * region of multiple countries, so I always picked one of the countries
     * in the specified region and used it's TZID.
     *
     * The Olson timezones are taken from https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
     */
    map.insert(QStringLiteral("Dateline Standard Time"), QStringLiteral("Pacific/Kwajalein"));    /* (GMT-12:00) International Date Line West */
    map.insert(QStringLiteral("Samoa Standard Time"), QStringLiteral("Pacific/Apia"));            /* (GMT-11:00) Midway Island, Samoa */
    map.insert(QStringLiteral("Hawaiian Standard Time"), QStringLiteral("Pacific/Honolulu"));     /* (GMT-10:00) Hawaii */
    map.insert(QStringLiteral("Alaskan Standard Time"), QStringLiteral("America/Anchorage"));     /* (GMT-09:00) Alaska */
    map.insert(QStringLiteral("Pacific Standard Time"), QStringLiteral("America/Los_Angeles"));   /* (GMT-08:00) Pacific Time (US and Canada); Tijuana */
    map.insert(QStringLiteral("Mountain Standard Time"), QStringLiteral("America/Denver"));       /* (GMT-07:00) Mountain Time (US and Canada) */
    map.insert(QStringLiteral("Mexico Standard Time 2"), QStringLiteral("America/Chihuahua"));    /* (GMT-07:00) Chihuahua, La Paz, Mazatlan */
    map.insert(QStringLiteral("U.S. Mountain Standard Time"), QStringLiteral("America/Phoenix")); /* (GMT-07:00) Arizona */
    map.insert(QStringLiteral("Central Standard Time"), QStringLiteral("America/Chicago"));       /* (GMT-06:00) Central Time (US and Canada */
    map.insert(QStringLiteral("Canada Central Standard Time"), QStringLiteral("America/Winnipeg"));       /* (GMT-06:00) Saskatchewan */
    map.insert(QStringLiteral("Mexico Standard Time"), QStringLiteral("America/Mexico_City"));    /* (GMT-06:00) Guadalajara, Mexico City, Monterrey */
    map.insert(QStringLiteral("Central America Standard Time"), QStringLiteral("America/Chicago"));       /* (GMT-06:00) Central America */
    map.insert(QStringLiteral("Eastern Standard Time"), QStringLiteral("America/New_York"));      /* (GMT-05:00) Eastern Time (US and Canada) */
    map.insert(QStringLiteral("U.S. Eastern Standard Time"), QStringLiteral("America/New_York")); /* (GMT-05:00) Indiana (East) */
    map.insert(QStringLiteral("S.A. Pacific Standard Time"), QStringLiteral("America/Bogota"));   /* (GMT-05:00) Bogota, Lima, Quito */
    map.insert(QStringLiteral("Atlantic Standard Time"), QStringLiteral("America/Halifax"));      /* (GMT-04:00) Atlantic Time (Canada) */
    map.insert(QStringLiteral("S.A. Western Standard Time"), QStringLiteral("America/La_Paz"));   /* (GMT-04:00) Caracas, La Paz */
    map.insert(QStringLiteral("Pacific S.A. Standard Time"), QStringLiteral("America/Santiago")); /* (GMT-04:00) Santiago */
    map.insert(QStringLiteral("Newfoundland and Labrador Standard Time"), QStringLiteral("America/St_Johns"));    /* (GMT-03:30) Newfoundland and Labrador */
    map.insert(QStringLiteral("E. South America Standard Time"), QStringLiteral("America/Brazil"));       /* (GMT-03:00) Brasilia */
    map.insert(QStringLiteral("S.A. Eastern Standard Time"), QStringLiteral("America/Argentina/Buenos_Aires"));   /* (GMT-03:00) Buenos Aires, Georgetown */
    map.insert(QStringLiteral("Greenland Standard Time"), QStringLiteral("America/Greenland"));   /* (GMT-03:00) Greenland */
    map.insert(QStringLiteral("Mid-Atlantic Standard Time"), QStringLiteral("America/Noronha"));  /* (GMT-02:00) Mid-Atlantic */
    map.insert(QStringLiteral("Azores Standard Time"), QStringLiteral("Atlantic/Portugal"));      /* (GMT-01:00) Azores */
    map.insert(QStringLiteral("Cape Verde Standard Time"), QStringLiteral("Atlantic/Cape_Verde"));        /* (GMT-01:00) Cape Verde Islands */
    map.insert(QStringLiteral("GMT Standard Time"), QStringLiteral("Europe/London"));             /* (GMT) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    map.insert(QStringLiteral("Greenwich Standard Time"), QStringLiteral("Africa/Casablanca"));   /* (GMT) Casablanca, Monrovia */
    map.insert(QStringLiteral("Central Europe Standard Time"), QStringLiteral("Europe/Prague"));  /* (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague */
    map.insert(QStringLiteral("Central European Standard Time"), QStringLiteral("Europe/Sarajevo"));      /* (GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb */
    map.insert(QStringLiteral("Romance Standard Time"), QStringLiteral("Europe/Brussels"));       /* (GMT+01:00) Brussels, Copenhagen, Madrid, Paris */
    map.insert(QStringLiteral("W. Europe Standard Time"), QStringLiteral("Europe/Amsterdam"));    /* (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    map.insert(QStringLiteral("W. Central Africa Standard Time"), QStringLiteral("Africa/Bangui"));       /* (GMT+01:00) West Central Africa */
    map.insert(QStringLiteral("E. Europe Standard Time"), QStringLiteral("Europe/Bucharest"));    /* (GMT+02:00) Bucharest */
    map.insert(QStringLiteral("Egypt Standard Time"), QStringLiteral("Africa/Cairo"));            /* (GMT+02:00) Cairo */
    map.insert(QStringLiteral("FLE Standard Time"), QStringLiteral("Europe/Helsinki"));           /* (GMT+02:00) Helsinki, Kiev, Riga, Sofia, Tallinn, Vilnius */
    map.insert(QStringLiteral("GTB Standard Time"), QStringLiteral("Europe/Athens"));             /* (GMT+02:00) Athens, Istanbul, Minsk */
    map.insert(QStringLiteral("Israel Standard Time"), QStringLiteral("Europe/Athens"));          /* (GMT+02:00) Jerusalem */
    map.insert(QStringLiteral("South Africa Standard Time"), QStringLiteral("Africa/Harare"));    /* (GMT+02:00) Harare, Pretoria */
    map.insert(QStringLiteral("Russian Standard Time"), QStringLiteral("Europe/Moscow"));         /* (GMT+03:00) Moscow, St. Petersburg, Volgograd */
    map.insert(QStringLiteral("Arab Standard Time"), QStringLiteral("Asia/Kuwait"));              /* (GMT+03:00) Kuwait, Riyadh */
    map.insert(QStringLiteral("E. Africa Standard Time"), QStringLiteral("Africa/Nairobi"));      /* (GMT+03:00) Nairobi */
    map.insert(QStringLiteral("Arabic Standard Time"), QStringLiteral("Asia/Baghdad"));           /* (GMT+03:00) Baghdad */
    map.insert(QStringLiteral("Iran Standard Time"), QStringLiteral("Asia/Tehran"));              /* (GMT+03:30) Tehran */
    map.insert(QStringLiteral("Arabian Standard Time"), QStringLiteral("Asia/Dubai"));            /* (GMT+04:00) Abu Dhabi, Muscat */
    map.insert(QStringLiteral("Caucasus Standard Time"), QStringLiteral("Asia/Tbilisi"));         /* (GMT+04:00) Baku, Tbilisi, Yerevan */
    map.insert(QStringLiteral("Transitional Islamic State of Afghanistan Standard Time"), QStringLiteral("Asia/Kabul"));  /* (GMT+04:30) Kabul */
    map.insert(QStringLiteral("Ekaterinburg Standard Time"), QStringLiteral("Asia/Yekaterinburg"));       /* (GMT+05:00) Ekaterinburg */
    map.insert(QStringLiteral("West Asia Standard Time"), QStringLiteral("Asia/Karachi"));        /* (GMT+05:00) Islamabad, Karachi, Tashkent */
    map.insert(QStringLiteral("India Standard Time"), QStringLiteral("Asia/Calcutta"));           /* (GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi */
    map.insert(QStringLiteral("Nepal Standard Time"), QStringLiteral("Asia/Calcutta"));           /* (GMT+05:45) Kathmandu */
    map.insert(QStringLiteral("Central Asia Standard Time"), QStringLiteral("Asia/Dhaka"));       /* (GMT+06:00) Astana, Dhaka */
    map.insert(QStringLiteral("Sri Lanka Standard Time"), QStringLiteral("Asia/Colombo"));        /* (GMT+06:00) Sri Jayawardenepura */
    map.insert(QStringLiteral("N. Central Asia Standard Time"), QStringLiteral("Asia/Omsk"));     /* (GMT+06:00) Almaty, Novosibirsk */
    map.insert(QStringLiteral("Myanmar Standard Time"), QStringLiteral("Asia/Rangoon"));          /* (GMT+06:30) Yangon Rangoon */
    map.insert(QStringLiteral("S.E. Asia Standard Time"), QStringLiteral("Asia/Bangkok"));        /* (GMT+07:00) Bangkok, Hanoi, Jakarta */
    map.insert(QStringLiteral("North Asia Standard Time"), QStringLiteral("Asia/Krasnoyarsk"));   /* (GMT+07:00) Krasnoyarsk */
    map.insert(QStringLiteral("China Standard Time"), QStringLiteral("Asia/Shanghai"));           /* (GMT+08:00) Beijing, Chongqing, Hong Kong SAR, Urumqi */
    map.insert(QStringLiteral("Singapore Standard Time"), QStringLiteral("Asia/Singapore"));      /* (GMT+08:00) Kuala Lumpur, Singapore */
    map.insert(QStringLiteral("Taipei Standard Time"), QStringLiteral("Asia/Taipei"));            /* (GMT+08:00) Taipei */
    map.insert(QStringLiteral("W. Australia Standard Time"), QStringLiteral("Australia/Perth"));  /* (GMT+08:00) Perth */
    map.insert(QStringLiteral("North Asia East Standard Time"), QStringLiteral("Asia/Irkutsk"));  /* (GMT+08:00) Irkutsk, Ulaanbaatar */
    map.insert(QStringLiteral("Korea Standard Time"), QStringLiteral("Asia/Seoul"));              /* (GMT+09:00) Seoul */
    map.insert(QStringLiteral("Tokyo Standard Time"), QStringLiteral("Asia/Tokyo"));              /* (GMT+09:00) Osaka, Sapporo, Tokyo */
    map.insert(QStringLiteral("Yakutsk Standard Time"), QStringLiteral("Asia/Yakutsk"));          /* (GMT+09:00) Yakutsk */
    map.insert(QStringLiteral("A.U.S. Central Standard Time"), QStringLiteral("Australia/Darwin"));       /* (GMT+09:30) Darwin */
    map.insert(QStringLiteral("Cen. Australia Standard Time"), QStringLiteral("Australia/Adelaide"));     /* (GMT+09:30) Adelaide */
    map.insert(QStringLiteral("A.U.S. Eastern Standard Time"), QStringLiteral("Australia/Sydney"));       /* (GMT+10:00) Canberra, Melbourne, Sydney */
    map.insert(QStringLiteral("E. Australia Standard Time"), QStringLiteral("Australia/Brisbane"));       /* (GMT+10:00) Brisbane */
    map.insert(QStringLiteral("Tasmania Standard Time"), QStringLiteral("Australia/Hobart"));     /* (GMT+10:00) Hobart */
    map.insert(QStringLiteral("Vladivostok Standard Time"), QStringLiteral("Asia/Vladivostok"));  /* (GMT+10:00) Vladivostok */
    map.insert(QStringLiteral("West Pacific Standard Time"), QStringLiteral("Pacific/Guam"));     /* (GMT+10:00) Guam, Port Moresby */
    map.insert(QStringLiteral("Central Pacific Standard Time"), QStringLiteral("Pacific/Noumea"));        /* (GMT+11:00) Magadan, Solomon Islands, New Caledonia */
    map.insert(QStringLiteral("Fiji Islands Standard Time"), QStringLiteral("Pacific/Fiji"));     /* (GMT+12:00) Fiji Islands, Kamchatka, Marshall Islands */
    map.insert(QStringLiteral("New Zealand Standard Time"), QStringLiteral("Pacific/Auckland"));  /* (GMT+12:00) Auckland, Wellington */
    map.insert(QStringLiteral("Tonga Standard Time"), QStringLiteral("Pacific/Tongatapu"));       /* (GMT+13:00) Nuku'alofa */
    map.insert(QStringLiteral("Azerbaijan Standard Time"), QStringLiteral("America/Argentina/Buenos_Aires"));     /* (GMT-03:00) Buenos Aires */
    map.insert(QStringLiteral("Middle East Standard Time"), QStringLiteral("Asia/Beirut"));       /* (GMT+02:00) Beirut */
    map.insert(QStringLiteral("Jordan Standard Time"), QStringLiteral("Asia/Amman"));             /* (GMT+02:00) Amman */
    map.insert(QStringLiteral("Central Standard Time (Mexico)"), QStringLiteral("America/Mexico_City"));  /* (GMT-06:00) Guadalajara, Mexico City, Monterrey - New */
    map.insert(QStringLiteral("Mountain Standard Time (Mexico)"), QStringLiteral("America/Ojinaga"));     /* (GMT-07:00) Chihuahua, La Paz, Mazatlan - New */
    map.insert(QStringLiteral("Pacific Standard Time (Mexico)"), QStringLiteral("America/Tijuana"));      /* (GMT-08:00) Tijuana, Baja California */
    map.insert(QStringLiteral("Namibia Standard Time"), QStringLiteral("Africa/Windhoek"));       /* (GMT+02:00) Windhoek */
    map.insert(QStringLiteral("Georgian Standard Time"), QStringLiteral("Asia/Tbilisi"));         /* (GMT+03:00) Tbilisi */
    map.insert(QStringLiteral("Central Brazilian Standard Time"), QStringLiteral("America/Manaus"));      /*(GMT-04:00) Manaus */
    map.insert(QStringLiteral("Montevideo Standard Time"), QStringLiteral("America/Montevideo"));        /* (GMT-03:00) Montevideo */
    map.insert(QStringLiteral("Armenian Standard Time"), QStringLiteral("Asia/Yerevan"));         /* (GMT+04:00) Yerevan */
    map.insert(QStringLiteral("Venezuela Standard Time"), QStringLiteral("America/Caracas"));     /* (GMT-04:30) Caracas */
    map.insert(QStringLiteral("Argentina Standard Time"), QStringLiteral("America/Argentina/Buenos_Aires"));      /* (GMT-03:00) Buenos Aires */
    map.insert(QStringLiteral("Morocco Standard Time"), QStringLiteral("Africa/Casablanca"));     /* (GMT) Casablanca */
    map.insert(QStringLiteral("Pakistan Standard Time"), QStringLiteral("Asia/Karachi"));         /* (GMT+05:00) Islamabad, Karachi */
    map.insert(QStringLiteral("Mauritius Standard Time"), QStringLiteral("Indian/Mauritius"));    /* (GMT+04:00) Port Louis */
    map.insert(QStringLiteral("UTC"), QStringLiteral("UTC"));                                     /* (GMT) Coordinated Universal Time */
    map.insert(QStringLiteral("Paraguay Standard Time"), QStringLiteral("America/Asuncion"));     /* (GMT-04:00) Asuncion */
    map.insert(QStringLiteral("Kamchatka Standard Time"), QStringLiteral("Asia/Kamchatka"));      /* (GMT+12:00) Petropavlovsk-Kamchatsky */

    return map;
}

static const QMap<int, QString> MSCDOTZIDTable = initMSCDOTZIDTable();
static const QMap<QString, QString> MSSTTZTable = initMSStandardTimeTZTable();

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
        if (property.startsWith(QStringLiteral("X-MICROSOFT-CDO-TZID"))) {
            QStringList parsed = property.split(QLatin1Char(':'));
            if (parsed.length() != 2) {
                break;
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
            if (tzid.contains(QStringLiteral("Dublin")) ||
                tzid.contains(QStringLiteral("Edinburgh")) ||
                tzid.contains(QStringLiteral("Lisbon")) ||
                tzid.contains(QStringLiteral("London")))
            {
                return QStringLiteral("Europe/London");
            }

            /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
            else if (tzid.contains(QStringLiteral("Sarajevo")) ||
                     tzid.contains(QStringLiteral("Skopje")) ||
                     tzid.contains(QStringLiteral("Sofija")) ||
                     tzid.contains(QStringLiteral("Vilnius")) ||
                     tzid.contains(QStringLiteral("Warsaw")) ||
                     tzid.contains(QStringLiteral("Zagreb")))
            {
                return QStringLiteral("Europe/Sarajevo");
            }
        }

        if (MSCDOTZIDTable.contains(CDOId)) {
            return MSCDOTZIDTable.value(CDOId);
        }
    }

    /* We failed to map to X-MICROSOFT-CDO-TZID. Let's try mapping the TZID
     * onto the Microsoft Standard Time Time Zones */
    if (MSSTTZTable.contains(tzid)) {
        return MSSTTZTable.value(tzid);
    }

    /* Fail/ Just return the original TZID and hope Google will accept it
     * (though we know it won't) */
    return tzid;
}

} // namespace CalendarService

} // namespace KGAPI2
