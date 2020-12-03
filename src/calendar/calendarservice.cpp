/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "calendarservice.h"
#include "calendar.h"
#include "event.h"
#include "reminder.h"
#include "utils.h"
#include "../debug.h"

#include <KCalendarCore/Alarm>
#include <KCalendarCore/Event>
#include <KCalendarCore/Attendee>
#include <KCalendarCore/Person>
#include <KCalendarCore/Recurrence>
#include <KCalendarCore/RecurrenceRule>
#include <KCalendarCore/ICalFormat>

#include <QJsonDocument>
#include <QUrlQuery>
#include <QTimeZone>
#include <QVariant>
#include <QNetworkRequest>

#include <map>
#include <memory>

namespace KGAPI2
{

namespace CalendarService
{

namespace Private
{
KCalendarCore::DateList parseRDate(const QString &rule);

ObjectPtr JSONToCalendar(const QVariantMap &data);
ObjectPtr JSONToEvent(const QVariantMap &data, const QString &timezone = QString());

/**
 * Checks whether TZID is in Olson format and converts it to it if necessary
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

static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));
static const QString CalendarListBasePath(QStringLiteral("/calendar/v3/users/me/calendarList"));
static const QString CalendarBasePath(QStringLiteral("/calendar/v3/calendars"));
}

QNetworkRequest prepareRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());

    return request;
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
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID);
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
    return url;
}

QUrl fetchEventUrl(const QString& calendarID, const QString& eventID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events/") % eventID);
    return url;
}

namespace {

QString sendUpdatesPolicyToString(SendUpdatesPolicy policy)
{
    switch (policy) {
    case SendUpdatesPolicy::All:
        return QStringLiteral("all");
    case SendUpdatesPolicy::ExternalOnly:
        return QStringLiteral("externalOnly");
    case SendUpdatesPolicy::None:
        return QStringLiteral("none");
    }
    Q_UNREACHABLE();
}

static const QString sendUpatesQueryParam = QStringLiteral("sendUpdates");
static const QString destinationQueryParam = QStringLiteral("destination");
}

QUrl updateEventUrl(const QString& calendarID, const QString& eventID, SendUpdatesPolicy updatePolicy)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events/") % eventID);
    QUrlQuery query(url);
    query.addQueryItem(sendUpatesQueryParam, sendUpdatesPolicyToString(updatePolicy));
    url.setQuery(query);
    return url;
}

QUrl createEventUrl(const QString& calendarID, SendUpdatesPolicy updatePolicy)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events"));
    QUrlQuery query(url);
    query.addQueryItem(sendUpatesQueryParam, sendUpdatesPolicyToString(updatePolicy));
    url.setQuery(query);
    return url;
}

QUrl importEventUrl(const QString& calendarID, SendUpdatesPolicy updatePolicy)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % calendarID % QLatin1String("/events") % QLatin1String("/import"));
    QUrlQuery query(url);
    query.addQueryItem(sendUpatesQueryParam, sendUpdatesPolicyToString(updatePolicy));
    url.setQuery(query);
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
    url.setPath(Private::CalendarBasePath % QLatin1Char('/') % sourceCalendar % QLatin1String("/events/") % eventID % QLatin1String("/move"));
    QUrlQuery query(url);
    query.addQueryItem(destinationQueryParam, destCalendar);
    url.setQuery(query);
    return url;
}

QUrl freeBusyQueryUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(QStringLiteral("/calendar/v3/freeBusy"));
    return url;
}

namespace {

static const auto kindParam = QStringLiteral("kind");
static const auto idParam = QStringLiteral("id");
static const auto etagParam = QStringLiteral("etag");

static const auto nextSyncTokenParam = QStringLiteral("nextSyncToken");
static const auto nextPageTokenParam = QStringLiteral("nextPageToken");
static const auto pageTokenParam = QStringLiteral("pageToken");
static const auto itemsParam = QStringLiteral("items");

static const auto calendarSummaryParam = QStringLiteral("summary");
static const auto calendarDescriptionParam = QStringLiteral("description");
static const auto calendarLocationParam = QStringLiteral("location");
static const auto calendarTimezoneParam = QStringLiteral("timeZone");
static const auto calendarBackgroundColorParam = QStringLiteral("backgroundColor");
static const auto calendarForegroundColorParam = QStringLiteral("foregroundColor");
static const auto calendarAccessRoleParam = QStringLiteral("accessRole");
static const auto calendarDefaultRemindersParam = QStringLiteral("defaultReminders");
static const auto reminderMethodParam = QStringLiteral("method");
static const auto reminderMinutesParam = QStringLiteral("minutes");

static const auto eventiCalUIDParam = QStringLiteral("iCalUID");
static const auto eventStatusParam = QStringLiteral("status");
static const auto eventCreatedParam = QStringLiteral("created");
static const auto eventUpdatedParam = QStringLiteral("updated");
static const auto eventSummaryParam = QStringLiteral("summary");
static const auto eventDescriptionParam = QStringLiteral("description");
static const auto eventLocationParam = QStringLiteral("location");
static const auto eventStartPram = QStringLiteral("start");
static const auto eventEndParam = QStringLiteral("end");
static const auto eventOriginalStartTimeParam = QStringLiteral("originalStartTime");
static const auto eventTransparencyParam = QStringLiteral("transparency");
static const auto eventOrganizerParam = QStringLiteral("organizer");
static const auto eventAttendeesParam = QStringLiteral("attendees");
static const auto eventRecurrenceParam = QStringLiteral("recurrence");
static const auto eventRemindersParam = QStringLiteral("reminders");
static const auto eventExtendedPropertiesParam = QStringLiteral("extendedProperties");
static const auto eventRecurringEventIdParam = QStringLiteral("recurringEventId");

static const auto attendeeDisplayNameParam = QStringLiteral("displayName");
static const auto attendeeEmailParam = QStringLiteral("email");
static const auto attendeeResponseStatusParam = QStringLiteral("responseStatus");
static const auto attendeeOptionalParam = QStringLiteral("optional");

static const auto organizerDisplayNameParam = QStringLiteral("displayName");
static const auto organizerEmailParam = QStringLiteral("email");

static const auto reminderUseDefaultParam = QStringLiteral("useDefault");
static const auto reminderOverridesParam = QStringLiteral("overrides");

static const auto propertyPrivateParam = QStringLiteral("private");
static const auto propertySharedParam = QStringLiteral("shared");

static const auto dateParam = QStringLiteral("date");
static const auto dateTimeParam = QStringLiteral("dateTime");
static const auto timeZoneParam = QStringLiteral("timeZone");


static const auto calendarListKind = QLatin1String("calendar#calendarList");
static const auto calendarListEntryKind = QLatin1String("calendar#calendarListEntry");
static const auto calendarKind = QLatin1String("calendar#calendar");
static const auto eventKind = QLatin1String("calendar#event");
static const auto eventsKind = QLatin1String("calendar#events");

static const auto writerAccessRole = QLatin1String("writer");
static const auto ownerAccessRole = QLatin1String("owner");
static const auto emailMethod = QLatin1String("email");
static const auto popupMethod = QLatin1String("popup");

static const auto confirmedStatus = QLatin1String("confirmed");
static const auto canceledStatus = QLatin1String("cancelled");
static const auto tentativeStatus = QLatin1String("tentative");
static const auto acceptedStatus = QLatin1String("accepted");
static const auto needsActionStatus = QLatin1String("needsAction");
static const auto transparentTransparency = QLatin1String("transparent");
static const auto opaqueTransparency = QLatin1String("opaque");
static const auto declinedStatus = QLatin1String("declined");
static const auto categoriesProperty = QLatin1String("categories");

}

QString APIVersion()
{
    return QStringLiteral("3");
}

CalendarPtr JSONToCalendar(const QByteArray& jsonData)
{
    const auto document = QJsonDocument::fromJson(jsonData);
    const auto calendar = document.toVariant().toMap();

    if (calendar.value(kindParam).toString() != calendarListEntryKind && calendar.value(kindParam).toString() != calendarKind) {
        return CalendarPtr();
    }

    return Private::JSONToCalendar(calendar).staticCast<Calendar>();
}

ObjectPtr Private::JSONToCalendar(const QVariantMap& data)
{
    auto calendar = CalendarPtr::create();

    const auto id = QUrl::fromPercentEncoding(data.value(idParam).toByteArray());
    calendar->setUid(id);
    calendar->setEtag(data.value(etagParam).toString());
    calendar->setTitle(data.value(calendarSummaryParam).toString());
    calendar->setDetails(data.value(calendarDescriptionParam).toString());
    calendar->setLocation(data.value(calendarLocationParam).toString());
    calendar->setTimezone(data.value(calendarTimezoneParam).toString());
    calendar->setBackgroundColor(QColor(data.value(calendarBackgroundColorParam).toString()));
    calendar->setForegroundColor(QColor(data.value(calendarForegroundColorParam).toString()));

    if ((data.value(calendarAccessRoleParam).toString() == writerAccessRole) ||
            (data.value(calendarAccessRoleParam).toString() == ownerAccessRole)) {
        calendar->setEditable(true);
    } else {
        calendar->setEditable(false);
    }

    const auto reminders = data.value(calendarDefaultRemindersParam).toList();
    for (const auto &r : reminders) {
        const auto reminder = r.toMap();

        auto rem = ReminderPtr::create();
        if (reminder.value(reminderMethodParam).toString() == emailMethod) {
            rem->setType(KCalendarCore::Alarm::Email);
        } else if (reminder.value(reminderMethodParam).toString() == popupMethod) {
            rem->setType(KCalendarCore::Alarm::Display);
        } else {
            rem->setType(KCalendarCore::Alarm::Invalid);
        }

        rem->setStartOffset(KCalendarCore::Duration(reminder.value(reminderMinutesParam).toInt() * (-60)));

        calendar->addDefaultReminer(rem);
    }

    return calendar.dynamicCast<Object>();
}

QByteArray calendarToJSON(const CalendarPtr& calendar)
{
    QVariantMap entry;

    if (!calendar->uid().isEmpty()) {
        entry.insert(idParam, calendar->uid());
    }

    entry.insert(calendarSummaryParam, calendar->title());
    entry.insert(calendarDescriptionParam, calendar->details());
    entry.insert(calendarLocationParam, calendar->location());
    if (!calendar->timezone().isEmpty()) {
        entry.insert(calendarTimezoneParam, calendar->timezone());
    }

    const auto document = QJsonDocument::fromVariant(entry);
    return document.toJson(QJsonDocument::Compact);
}


ObjectsList parseCalendarJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    const auto document = QJsonDocument::fromJson(jsonFeed);
    const auto data = document.toVariant().toMap();

    ObjectsList list;

    if (data.value(kindParam).toString() == calendarListKind) {
        if (data.contains(nextPageTokenParam)) {
            feedData.nextPageUrl = fetchCalendarsUrl();
            QUrlQuery query(feedData.nextPageUrl);
            query.addQueryItem(pageTokenParam, data.value(nextPageTokenParam).toString());
            feedData.nextPageUrl.setQuery(query);
        }
    } else {
        return {};
    }

    const auto items = data.value(itemsParam).toList();
    list.reserve(items.size());
    for (const auto &i : items) {
        list.push_back(Private::JSONToCalendar(i.toMap()));
    }

    return list;
}

EventPtr JSONToEvent(const QByteArray& jsonData)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(KGAPIDebug) << "Error parsing event JSON: " << error.errorString();
    }
    QVariantMap data = document.toVariant().toMap();
    if (data.value(kindParam).toString() != eventKind) {
        return EventPtr();
    }

    return Private::JSONToEvent(data).staticCast<Event>();
}

namespace {

struct ParsedDt {
    QDateTime dt;
    bool isAllDay;
};

ParsedDt parseDt(const QVariantMap &data, const QString &timezone, bool isDtEnd)
{
    if (data.contains(dateParam)) {
        auto dt = QDateTime::fromString(data.value(dateParam).toString(), Qt::ISODate);
        if (isDtEnd) {
            // Google reports all-day events to end on the next day, e.g. a
            // Monday all-day event will be reporting as starting on Monday and
            // ending on Tuesday, while KCalendarCore/iCal uses the same day for
            // dtEnd, so adjust the end date here.
            dt = dt.addDays(-1);
        }
        return {dt, true};
    } else if (data.contains(dateTimeParam)) {
        auto dt = Utils::rfc3339DateFromString(data.value(dateTimeParam).toString());
        // If there's a timezone specified in the "start" entity, then use it
        if (data.contains(timeZoneParam)) {
            const QTimeZone tz = QTimeZone(data.value(timeZoneParam).toString().toUtf8());
            if (tz.isValid()) {
                dt = dt.toTimeZone(tz);
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << data.value(timeZoneParam).toString();
            }

            // Otherwise try to fallback to calendar-wide timezone
        } else if (!timezone.isEmpty()) {
            const QTimeZone tz(timezone.toUtf8());
            if (tz.isValid()) {
                dt.setTimeZone(tz);
            } else {
                qCWarning(KGAPIDebug) << "Invalid timezone" << timezone;
            }
        }
        return {dt, false};
    } else {
        return {{}, false};
    }
}

void setEventCategories(EventPtr &event, const QVariantMap &properties)
{
    for (auto iter = properties.cbegin(), end = properties.cend(); iter != end; ++iter) {
        if (iter.key() == categoriesProperty) {
            event->setCategories(iter.value().toString());
        }
    }
}

} // namespace

ObjectPtr Private::JSONToEvent(const QVariantMap& data, const QString &timezone)
{
    auto event = EventPtr::create();

    event->setId(data.value(idParam).toString());
    event->setUid(data.value(eventiCalUIDParam).toString());
    event->setEtag(data.value(etagParam).toString());

    if (data.value(eventStatusParam).toString() == confirmedStatus) {
        event->setStatus(KCalendarCore::Incidence::StatusConfirmed);
    } else if (data.value(eventStatusParam).toString() == canceledStatus) {
        event->setStatus(KCalendarCore::Incidence::StatusCanceled);
        event->setDeleted(true);
    } else if (data.value(eventStatusParam).toString() == tentativeStatus) {
        event->setStatus(KCalendarCore::Incidence::StatusTentative);
    } else {
        event->setStatus(KCalendarCore::Incidence::StatusNone);
    }

    event->setCreated(Utils::rfc3339DateFromString(data.value(eventCreatedParam).toString()));
    event->setLastModified(Utils::rfc3339DateFromString(data.value(eventUpdatedParam).toString()));
    event->setSummary(data.value(eventSummaryParam).toString());
    event->setDescription(data.value(eventDescriptionParam).toString());
    event->setLocation(data.value(eventLocationParam).toString());

    const auto dtStart = parseDt(data.value(eventStartPram).toMap(), timezone, false);
    event->setDtStart(dtStart.dt);
    event->setAllDay(dtStart.isAllDay);

    const auto dtEnd = parseDt(data.value(eventEndParam).toMap(), timezone, true);
    event->setDtEnd(dtEnd.dt);

    if (data.contains(eventOriginalStartTimeParam)) {
        const auto recurrenceId = parseDt(data.value(eventOriginalStartTimeParam).toMap(), timezone, false);
        event->setRecurrenceId(recurrenceId.dt);
    }

    if (data.value(eventTransparencyParam).toString() == transparentTransparency) {
        event->setTransparency(Event::Transparent);
    } else { /* Assume opaque as default transparency */
        event->setTransparency(Event::Opaque);
    }

    const auto attendees = data.value(eventAttendeesParam).toList();
    for (const auto &a : attendees) {
        const auto att = a.toMap();
        KCalendarCore::Attendee attendee(
                        att.value(attendeeDisplayNameParam).toString(),
                        att.value(attendeeEmailParam).toString());
        const auto responseStatus = att.value(attendeeResponseStatusParam).toString();
        if (responseStatus == acceptedStatus) {
            attendee.setStatus(KCalendarCore::Attendee::Accepted);
        } else if (responseStatus == declinedStatus) {
            attendee.setStatus(KCalendarCore::Attendee::Declined);
        } else if (responseStatus == tentativeStatus) {
            attendee.setStatus(KCalendarCore::Attendee::Tentative);
        } else {
            attendee.setStatus(KCalendarCore::Attendee::NeedsAction);
        }

        if (att.value(attendeeOptionalParam).toBool()) {
            attendee.setRole(KCalendarCore::Attendee::OptParticipant);
        }
        const auto uid = att.value(idParam).toString();
        if (!uid.isEmpty()) {
            attendee.setUid(uid);
        } else {
            // Set some UID, just so that the results are reproducible
            attendee.setUid(QString::number(qHash(attendee.email())));
        }
        event->addAttendee(attendee, true);
    }

    /* According to RFC, only events with attendees can have an organizer.
     * Google seems to ignore it, so we must take care of it here */
    if (event->attendeeCount() > 0) {
        KCalendarCore::Person organizer;
        const auto organizerData = data.value(eventOrganizerParam).toMap();
        organizer.setName(organizerData.value(organizerDisplayNameParam).toString());
        organizer.setEmail(organizerData.value(organizerEmailParam).toString());
        event->setOrganizer(organizer);
    }

    const QStringList recrs = data.value(eventRecurrenceParam).toStringList();
    for (const QString & rec : recrs) {
        KCalendarCore::ICalFormat format;
        if (rec.leftRef(5) == QLatin1String("RRULE")) {
            auto recurrenceRule = std::make_unique<KCalendarCore::RecurrenceRule>();
            const auto ok = format.fromString(recurrenceRule.get(), rec.mid(6)); Q_UNUSED(ok);
            recurrenceRule->setRRule(rec);
            event->recurrence()->addRRule(recurrenceRule.release());
        } else if (rec.leftRef(6) == QLatin1String("EXRULE")) {
            auto recurrenceRule = std::make_unique<KCalendarCore::RecurrenceRule>();
            const auto ok = format.fromString(recurrenceRule.get(), rec.mid(7)); Q_UNUSED(ok);
            recurrenceRule->setRRule(rec);
            event->recurrence()->addExRule(recurrenceRule.release());
        } else if (rec.leftRef(6) == QLatin1String("EXDATE")) {
            KCalendarCore::DateList exdates = Private::parseRDate(rec);
            event->recurrence()->setExDates(exdates);
        } else if (rec.leftRef(5) == QLatin1String("RDATE")) {
            KCalendarCore::DateList rdates = Private::parseRDate(rec);
            event->recurrence()->setRDates(rdates);
        }
    }

    const auto reminders = data.value(eventRemindersParam).toMap();
    if (reminders.contains(reminderUseDefaultParam) && reminders.value(reminderUseDefaultParam).toBool()) {
        event->setUseDefaultReminders(true);
    } else {
        event->setUseDefaultReminders(false);
    }

    const auto overrides = reminders.value(reminderOverridesParam).toList();
    for (const auto &r : overrides) {
        const auto reminderOverride = r.toMap();
        auto alarm = KCalendarCore::Alarm::Ptr::create(static_cast<KCalendarCore::Incidence*>(event.data()));
        alarm->setTime(event->dtStart());

        if (reminderOverride.value(reminderMethodParam).toString() == popupMethod) {
            alarm->setType(KCalendarCore::Alarm::Display);
        } else if (reminderOverride.value(reminderMethodParam).toString() == emailMethod) {
            alarm->setType(KCalendarCore::Alarm::Email);
        } else {
            alarm->setType(KCalendarCore::Alarm::Invalid);
            continue;
        }

        alarm->setStartOffset(KCalendarCore::Duration(reminderOverride.value(reminderMinutesParam).toInt() * (-60)));
        alarm->setEnabled(true);
        event->addAlarm(alarm);
    }

    const auto extendedProperties = data.value(eventExtendedPropertiesParam).toMap();
    setEventCategories(event, extendedProperties.value(propertyPrivateParam).toMap());
    setEventCategories(event, extendedProperties.value(propertySharedParam).toMap());

    return event.dynamicCast<Object>();
}

namespace {

enum class SerializeDtFlag {
    AllDay =        1 << 0,
    IsDtEnd =       1 << 1,
    HasRecurrence = 1 << 2
};
using SerializeDtFlags = QFlags<SerializeDtFlag>;

QVariantMap serializeDt(const EventPtr &event, const QDateTime &dt, SerializeDtFlags flags)
{
    QVariantMap rv;
    if (flags & SerializeDtFlag::AllDay) {
        /* For Google, all-day events starts on Monday and ends on Tuesday,
         * while in KDE, it both starts and ends on Monday. */
        const auto adjusted = dt.addDays(flags & SerializeDtFlag::IsDtEnd ? 1 : 0);
        rv.insert(dateParam, adjusted.toString(QStringLiteral("yyyy-MM-dd")));
    } else {
        rv.insert(dateTimeParam, Utils::rfc3339DateToString(dt));
        QString tzEnd = QString::fromUtf8(dt.timeZone().id());
        if (flags & SerializeDtFlag::HasRecurrence && tzEnd.isEmpty()) {
            tzEnd = QString::fromUtf8(QTimeZone::utc().id());
        }
        if (!tzEnd.isEmpty()) {
            rv.insert(timeZoneParam, Private::checkAndConverCDOTZID(tzEnd, event));
        }
    }

    return rv;
}

} // namespace

QByteArray eventToJSON(const EventPtr& event, EventSerializeFlags flags)
{
    QVariantMap data;

    data.insert(kindParam, eventKind);

    if (!(flags & EventSerializeFlag::NoID)) {
        data.insert(idParam, event->id());
    }

    data.insert(eventiCalUIDParam, event->uid());

    if (event->status() == KCalendarCore::Incidence::StatusConfirmed) {
        data.insert(eventStatusParam, confirmedStatus);
    } else if (event->status() == KCalendarCore::Incidence::StatusCanceled) {
        data.insert(eventStatusParam, canceledStatus);
    } else if (event->status() == KCalendarCore::Incidence::StatusTentative) {
        data.insert(eventStatusParam, tentativeStatus);
    }

    data.insert(eventSummaryParam, event->summary());
    data.insert(eventDescriptionParam, event->description());
    data.insert(eventLocationParam, event->location());

    QVariantList recurrence;
    KCalendarCore::ICalFormat format;
    const auto exRules = event->recurrence()->exRules();
    const auto rRules = event->recurrence()->rRules();
    recurrence.reserve(rRules.size() + rRules.size() + 2);
    for (KCalendarCore::RecurrenceRule *rRule : rRules) {
        recurrence.push_back(format.toString(rRule).remove(QStringLiteral("\r\n")));
    }
    for (KCalendarCore::RecurrenceRule *rRule : exRules) {
        recurrence.push_back(format.toString(rRule).remove(QStringLiteral("\r\n")));
    }

    QStringList dates;
    const auto rDates = event->recurrence()->rDates();
    dates.reserve(rDates.size());
    for (const auto &rDate : rDates) {
        dates.push_back(rDate.toString(QStringLiteral("yyyyMMdd")));
    }

    if (!dates.isEmpty()) {
        recurrence.push_back(QString(QStringLiteral("RDATE;VALUE=DATA:") + dates.join(QLatin1Char(','))));
    }

    dates.clear();
    const auto exDates = event->recurrence()->exDates();
    dates.reserve(exDates.size());
    for (const auto &exDate : exDates) {
        dates.push_back(exDate.toString(QStringLiteral("yyyyMMdd")));
    }

    if (!dates.isEmpty()) {
        recurrence.push_back(QString(QStringLiteral("EXDATE;VALUE=DATE:") + dates.join(QLatin1Char(','))));
    }

    if (!recurrence.isEmpty()) {
        data.insert(eventRecurrenceParam, recurrence);
    }

    SerializeDtFlags dtFlags;
    if (event->allDay()) {
        dtFlags |= SerializeDtFlag::AllDay;
    }
    if (!recurrence.isEmpty()) {
        dtFlags |= SerializeDtFlag::HasRecurrence;
    }

    data.insert(eventStartPram, serializeDt(event, event->dtStart(), dtFlags));
    data.insert(eventEndParam, serializeDt(event, event->dtEnd(), dtFlags | SerializeDtFlag::IsDtEnd));

    if (event->hasRecurrenceId()) {
        data.insert(eventOrganizerParam, serializeDt(event, event->recurrenceId(), dtFlags));
        data.insert(eventRecurringEventIdParam, event->id());
    }

    if (event->transparency() == Event::Transparent) {
        data.insert(eventTransparencyParam, transparentTransparency);
    } else {
        data.insert(eventTransparencyParam, opaqueTransparency);
    }

    QVariantList atts;
    const auto attendees = event->attendees();
    for (const auto &attee : attendees) {
        QVariantMap att{{attendeeDisplayNameParam, attee.name()},
                        {attendeeEmailParam, attee.email()}};

        if (attee.status() == KCalendarCore::Attendee::Accepted) {
            att.insert(attendeeResponseStatusParam, acceptedStatus);
        } else if (attee.status() == KCalendarCore::Attendee::Declined) {
            att.insert(attendeeResponseStatusParam, declinedStatus);
        } else if (attee.status() == KCalendarCore::Attendee::Tentative) {
            att.insert(attendeeResponseStatusParam, tentativeStatus);
        } else {
            att.insert(attendeeResponseStatusParam, needsActionStatus);
        }

        if (attee.role() == KCalendarCore::Attendee::OptParticipant) {
            att.insert(attendeeOptionalParam, true);
        }
        if (!attee.uid().isEmpty()) {
            att.insert(idParam, attee.uid());
        }
        atts.append(att);
    }

    if (!atts.isEmpty()) {
        data.insert(eventAttendeesParam, atts);

        /* According to RFC, event without attendees should not have
         * any organizer. */
        const auto organizer = event->organizer();
        if (!organizer.isEmpty()) {
            data.insert(eventOrganizerParam,
                        QVariantMap{{organizerDisplayNameParam, organizer.fullName()},
                                    {organizerEmailParam, organizer.email()}});
        }
    }

    QVariantList overrides;
    const auto alarms = event->alarms();
    for (const auto &alarm : alarms) {
        QVariantMap reminderOverride;
        if (alarm->type() == KCalendarCore::Alarm::Display) {
            reminderOverride.insert(reminderMethodParam, popupMethod);
        } else if (alarm->type() == KCalendarCore::Alarm::Email) {
            reminderOverride.insert(reminderMethodParam, emailMethod);
        } else {
            continue;
        }
        reminderOverride.insert(reminderMinutesParam, (int)(alarm->startOffset().asSeconds() / -60));

        overrides.push_back(reminderOverride);
    }

    data.insert(eventRemindersParam,
                QVariantMap{{reminderUseDefaultParam, false},
                            {reminderOverridesParam, overrides}});

    if (!event->categories().isEmpty()) {
        data.insert(eventExtendedPropertiesParam,
                    QVariantMap{{propertySharedParam, QVariantMap{{categoriesProperty, event->categoriesStr()}}}});
    }

    /* TODO: Implement support for additional features:
     * https://developers.google.com/gdata/docs/2.0/elements?csw=1
     */

    const auto document = QJsonDocument::fromVariant(data);
    return document.toJson(QJsonDocument::Compact);
}

ObjectsList parseEventJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    const auto document = QJsonDocument::fromJson(jsonFeed);
    const auto data = document.toVariant().toMap();

    QString timezone;
    if (data.value(kindParam) == eventsKind) {
        if (data.contains(nextPageTokenParam)) {
            QString calendarId = feedData.requestUrl.toString().remove(QStringLiteral("https://www.googleapis.com/calendar/v3/calendars/"));
            calendarId = calendarId.left(calendarId.indexOf(QLatin1Char('/')));
            feedData.nextPageUrl = feedData.requestUrl;
            // replace the old pageToken with a new one
            QUrlQuery query(feedData.nextPageUrl);
            query.removeQueryItem(pageTokenParam);
            query.addQueryItem(pageTokenParam, data.value(nextPageTokenParam).toString());
            feedData.nextPageUrl.setQuery(query);
        }
        if (data.contains(timeZoneParam)) {
            // This should always be in Olson format
            timezone = data.value(timeZoneParam).toString();
        }
        if (data.contains(nextSyncTokenParam)) {
            feedData.syncToken = data[nextSyncTokenParam].toString();
        }
    } else {
        return {};
    }

    ObjectsList list;
    const auto items = data.value(itemsParam).toList();
    list.reserve(items.size());
    for (const auto &i : items) {
        list.push_back(Private::JSONToEvent(i.toMap(), timezone));
    }

    return list;
}

/******************************** PRIVATE ***************************************/

KCalendarCore::DateList Private::parseRDate(const QString& rule)
{
    KCalendarCore::DateList list;
    QStringRef value;
    QTimeZone tz;

    const auto left = rule.leftRef(rule.indexOf(QLatin1Char(':')));
    const auto params = left.split(QLatin1Char(';'));
    for (const auto &param : params) {
        if (param.startsWith(QLatin1String("VALUE"))) {
            value = param.mid(param.indexOf(QLatin1Char('=')) + 1);
        } else if (param.startsWith(QLatin1String("TZID"))) {
            auto _name = param.mid(param.indexOf(QLatin1Char('=')) + 1);
            tz = QTimeZone(_name.toUtf8());
        }
    }

    const auto datesStr = rule.midRef(rule.lastIndexOf(QLatin1Char(':')) + 1);
    const auto dates = datesStr.split(QLatin1Char(','));
    for (const auto &date : dates) {
        QDate dt;

        if (value == QLatin1String("DATE")) {
            dt = QDate::fromString(date.toString(), QStringLiteral("yyyyMMdd"));
        } else if (value == QLatin1String("PERIOD")) {
            const auto start = date.left(date.indexOf(QLatin1Char('/')));
            QDateTime kdt = Utils::rfc3339DateFromString(start.toString());
            if (tz.isValid()) {
                kdt.setTimeZone(tz);
            }

            dt = kdt.date();
        } else {
            QDateTime kdt = Utils::rfc3339DateFromString(date.toString());
            if (tz.isValid()) {
                kdt.setTimeZone(tz);
            }

            dt = kdt.date();
        }

        list.push_back(dt);
    }

    return list;
}

namespace {

/* Based on "Time Zone to CdoTimeZoneId Map"
 * https://docs.microsoft.com/en-us/previous-versions/office/developer/exchange-server-2007/aa563018(v=exchg.80)
 *
 * The mapping is not exact, since the CdoTimeZoneId usually refers to a
 * region of multiple countries, so I always picked one of the countries
 * in the specified region and used it's TZID.
 */
static const std::map<int, QLatin1String> MSCDOTZIDTable = {
    {0,  QLatin1String("UTC")},
    {1,  QLatin1String("Europe/London")},             /* GMT Greenwich Mean Time; Dublin, Edinburgh, London */
    /* Seriously? *sigh* Let's handle these two in checkAndConvertCDOTZID() */
    //{2, QLatin1String("Europe/Lisbon")},              /* GMT Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    //{2, QLatin1String("Europe/Sarajevo")},            /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
    {3,  QLatin1String("Europe/Paris")},              /* GMT+01:00 Paris, Madrid, Brussels, Copenhagen */
    {4,  QLatin1String("Europe/Berlin")},             /* GMT+01:00 Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    {5,  QLatin1String("Europe/Bucharest")},          /* GMT+02:00 Bucharest */
    {6,  QLatin1String("Europe/Prague")},             /* GMT+01:00 Prague, Central Europe */
    {7,  QLatin1String("Europe/Athens")},             /* GMT+02:00 Athens, Istanbul, Minsk */
    {8,  QLatin1String("America/Brazil")},            /* GMT-03:00 Brasilia */
    {9,  QLatin1String("America/Halifax")},           /* GMT-04:00 Atlantic time (Canada) */
    {10, QLatin1String("America/New_York")},          /* GMT-05:00 Eastern Time (US & Canada) */
    {11, QLatin1String("America/Chicago")},           /* GMT-06:00 Central Time (US & Canada) */
    {12, QLatin1String("America/Denver")},            /* GMT-07:00 Mountain Time (US & Canada) */
    {13, QLatin1String("America/Los_Angeles")},       /* GMT-08:00 Pacific Time (US & Canada); Tijuana */
    {14, QLatin1String("America/Anchorage")},         /* GMT-09:00 Alaska */
    {15, QLatin1String("Pacific/Honolulu")},          /* GMT-10:00 Hawaii */
    {16, QLatin1String("Pacific/Apia")},              /* GMT-11:00 Midway Island, Samoa */
    {17, QLatin1String("Pacific/Auckland")},          /* GMT+12:00 Auckland, Wellington */
    {18, QLatin1String("Australia/Brisbane")},        /* GMT+10:00 Brisbane, East Australia */
    {19, QLatin1String("Australia/Adelaide")},        /* GMT+09:30 Adelaide, Central Australia */
    {20, QLatin1String("Asia/Tokyo")},                /* GMT+09:00 Osaka, Sapporo, Tokyo */
    {21, QLatin1String("Asia/Singapore")},            /* GMT+08:00 Kuala Lumpur, Singapore */
    {22, QLatin1String("Asia/Bangkok")},              /* GMT+07:00 Bangkok, Hanoi, Jakarta */
    {23, QLatin1String("Asia/Calcutta")},             /* GMT+05:30 Kolkata, Chennai, Mumbai, New Delhi, India Standard Time */
    {24, QLatin1String("Asia/Dubai")},                /* GMT+04:00 Abu Dhabi, Muscat */
    {25, QLatin1String("Asia/Tehran")},               /* GMT+03:30 Tehran */
    {26, QLatin1String("Asia/Baghdad")},              /* GMT+03:00 Baghdad */
    {27, QLatin1String("Asia/Jerusalem")},            /* GMT+02:00 Israel, Jerusalem Standard Time */
    {28, QLatin1String("America/St_Johns")},          /* GMT-03:30 Newfoundland */
    {29, QLatin1String("Atlantic/Portugal")},         /* GMT-01:00 Azores */
    {30, QLatin1String("America/Noronha")},           /* GMT-02:00 Mid-Atlantic */
    {31, QLatin1String("Africa/Monrovia")},           /* GMT Casablanca, Monrovia */
    {32, QLatin1String("America/Argentina/Buenos_Aires")}, /* GMT-03:00 Buenos Aires, Georgetown */
    {33, QLatin1String("America/La_Paz")},            /* GMT-04:00 Caracas, La Paz */
    {34, QLatin1String("America/New_York")},          /* GMT-05:00 Indiana (East) */
    {35, QLatin1String("America/Bogota")},            /* GMT-05:00 Bogota, Lima, Quito */
    {36, QLatin1String("America/Winnipeg")},          /* GMT-06:00 Saskatchewan */
    {37, QLatin1String("America/Mexico_City")},       /* GMT-06:00 Mexico City, Tegucigalpa */
    {38, QLatin1String("America/Phoenix")},           /* GMT-07:00 Arizona */
    {39, QLatin1String("Pacific/Kwajalein")},         /* GMT-12:00 Eniwetok, Kwajalein, Dateline Time */
    {40, QLatin1String("Pacific/Fiji")},              /* GMT+12:00 Fušál, Kamchatka, Mashall Is. */
    {41, QLatin1String("Pacific/Noumea")},            /* GMT+11:00 Magadan, Solomon Is., New Caledonia */
    {42, QLatin1String("Australia/Hobart")},          /* GMT+10:00 Hobart, Tasmania */
    {43, QLatin1String("Pacific/Guam")},              /* GMT+10:00 Guam, Port Moresby */
    {44, QLatin1String("Australia/Darwin")},          /* GMT+09:30 Darwin */
    {45, QLatin1String("Asia/Shanghai")},             /* GMT+08:00 Beijing, Chongqing, Hong Kong SAR, Urumqi */
    {46, QLatin1String("Asia/Omsk")},                 /* GMT+06:00 Almaty, Novosibirsk, North Central Asia */
    {47, QLatin1String("Asia/Karachi")},              /* GMT+05:00 Islamabad, Karachi, Tashkent */
    {48, QLatin1String("Asia/Kabul")},                /* GMT+04:30 Kabul */
    {49, QLatin1String("Africa/Cairo")},              /* GMT+02:00 Cairo */
    {50, QLatin1String("Africa/Harare")},             /* GMT+02:00 Harare, Pretoria */
    {51, QLatin1String("Europe/Moscow")},             /* GMT+03:00 Moscow, St. Petersburg, Volgograd */
    {53, QLatin1String("Atlantic/Cape_Verde")},       /* GMT-01:00 Cape Verde Is. */
    {54, QLatin1String("Asia/Tbilisi")},              /* GMT+04:00 Baku, Tbilisi, Yerevan */
    {55, QLatin1String("America/Tegucigalpa")},       /* GMT-06:00 Central America */
    {56, QLatin1String("Africa/Nairobi")},            /* GMT+03:00 East Africa, Nairobi */
    {58, QLatin1String("Asia/Yekaterinburg")},        /* GMT+05:00 Ekaterinburg */
    {59, QLatin1String("Europe/Helsinki")},           /* GMT+02:00 Helsinki, Riga, Tallinn */
    {60, QLatin1String("America/Greenland")},         /* GMT-03:00 Greenland */
    {61, QLatin1String("Asia/Rangoon")},              /* GMT+06:30 Yangon (Rangoon) */
    {62, QLatin1String("Asia/Katmandu")},             /* GMT+05:45 Kathmandu, Nepal */
    {63, QLatin1String("Asia/Irkutsk")},              /* GMT+08:00 Irkutsk, Ulaan Bataar */
    {64, QLatin1String("Asia/Krasnoyarsk")},          /* GMT+07:00 Krasnoyarsk */
    {65, QLatin1String("America/Santiago")},          /* GMT-04:00 Santiago */
    {66, QLatin1String("Asia/Colombo")},              /* GMT+06:00 Sri Jayawardenepura, Sri Lanka */
    {67, QLatin1String("Pacific/Tongatapu")},         /* GMT+13:00 Nuku'alofa, Tonga */
    {68, QLatin1String("Asia/Vladivostok")},          /* GMT+10:00 Vladivostok */
    {69, QLatin1String("Africa/Bangui")},             /* GMT+01:00 West Central Africa */
    {70, QLatin1String("Asia/Yakutsk")},              /* GMT+09:00 Yakutsk */
    {71, QLatin1String("Asia/Dhaka")},                /* GMT+06:00 Astana, Dhaka */
    {72, QLatin1String("Asia/Seoul")},                /* GMT+09:00 Seoul, Korea Standard time */
    {73, QLatin1String("Australia/Perth")},           /* GMT+08:00 Perth, Western Australia */
    {74, QLatin1String("Asia/Kuwait")},               /* GMT+03:00 Arab, Kuwait, Riyadh */
    {75, QLatin1String("Asia/Taipei")},               /* GMT+08:00 Taipei */
    {76, QLatin1String("Australia/Sydney")}          /* GMT+10:00 Canberra, Melbourne, Sydney */
};

/* Based on "Microsoft Time Zone Index Values"
 * https://support.microsoft.com/en-gb/help/973627/microsoft-time-zone-index-values
 *
 * The mapping is not exact, since the TZID usually refers to a
 * region of multiple countries, so I always picked one of the countries
 * in the specified region and used it's TZID.
 *
 * The Olson timezones are taken from https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
 *
 * Note: using std::map, because it allows heterogeneous lookup, i.e. I can lookup the QLatin1String
 * keys by using QString value, which is not possible with Qt containers.
 */
static const std::map<QLatin1String, QLatin1String, std::less<>> MSSTTZTable = {
    {QLatin1String("Dateline Standard Time"), QLatin1String("Pacific/Kwajalein")},    /* (GMT-12:00) International Date Line West */
    {QLatin1String("Samoa Standard Time"), QLatin1String("Pacific/Apia")},            /* (GMT-11:00) Midway Island, Samoa */
    {QLatin1String("Hawaiian Standard Time"), QLatin1String("Pacific/Honolulu")},     /* (GMT-10:00) Hawaii */
    {QLatin1String("Alaskan Standard Time"), QLatin1String("America/Anchorage")},     /* (GMT-09:00) Alaska */
    {QLatin1String("Pacific Standard Time"), QLatin1String("America/Los_Angeles")},   /* (GMT-08:00) Pacific Time (US and Canada); Tijuana */
    {QLatin1String("Mountain Standard Time"), QLatin1String("America/Denver")},       /* (GMT-07:00) Mountain Time (US and Canada) */
    {QLatin1String("Mexico Standard Time 2"), QLatin1String("America/Chihuahua")},    /* (GMT-07:00) Chihuahua, La Paz, Mazatlan */
    {QLatin1String("U.S. Mountain Standard Time"), QLatin1String("America/Phoenix")}, /* (GMT-07:00) Arizona */
    {QLatin1String("Central Standard Time"), QLatin1String("America/Chicago")},       /* (GMT-06:00) Central Time (US and Canada */
    {QLatin1String("Canada Central Standard Time"), QLatin1String("America/Winnipeg")},       /* (GMT-06:00) Saskatchewan */
    {QLatin1String("Mexico Standard Time"), QLatin1String("America/Mexico_City")},    /* (GMT-06:00) Guadalajara, Mexico City, Monterrey */
    {QLatin1String("Central America Standard Time"), QLatin1String("America/Chicago")},       /* (GMT-06:00) Central America */
    {QLatin1String("Eastern Standard Time"), QLatin1String("America/New_York")},      /* (GMT-05:00) Eastern Time (US and Canada) */
    {QLatin1String("U.S. Eastern Standard Time"), QLatin1String("America/New_York")}, /* (GMT-05:00) Indiana (East) */
    {QLatin1String("S.A. Pacific Standard Time"), QLatin1String("America/Bogota")},   /* (GMT-05:00) Bogota, Lima, Quito */
    {QLatin1String("Atlantic Standard Time"), QLatin1String("America/Halifax")},      /* (GMT-04:00) Atlantic Time (Canada) */
    {QLatin1String("S.A. Western Standard Time"), QLatin1String("America/La_Paz")},   /* (GMT-04:00) Caracas, La Paz */
    {QLatin1String("Pacific S.A. Standard Time"), QLatin1String("America/Santiago")}, /* (GMT-04:00) Santiago */
    {QLatin1String("Newfoundland and Labrador Standard Time"), QLatin1String("America/St_Johns")},    /* (GMT-03:30) Newfoundland and Labrador */
    {QLatin1String("E. South America Standard Time"), QLatin1String("America/Brazil")},       /* (GMT-03:00) Brasilia */
    {QLatin1String("S.A. Eastern Standard Time"), QLatin1String("America/Argentina/Buenos_Aires")},   /* (GMT-03:00) Buenos Aires, Georgetown */
    {QLatin1String("Greenland Standard Time"), QLatin1String("America/Greenland")},   /* (GMT-03:00) Greenland */
    {QLatin1String("Mid-Atlantic Standard Time"), QLatin1String("America/Noronha")},  /* (GMT-02:00) Mid-Atlantic */
    {QLatin1String("Azores Standard Time"), QLatin1String("Atlantic/Portugal")},      /* (GMT-01:00) Azores */
    {QLatin1String("Cape Verde Standard Time"), QLatin1String("Atlantic/Cape_Verde")},        /* (GMT-01:00) Cape Verde Islands */
    {QLatin1String("GMT Standard Time"), QLatin1String("Europe/London")},             /* (GMT) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    {QLatin1String("Greenwich Standard Time"), QLatin1String("Africa/Casablanca")},   /* (GMT) Casablanca, Monrovia */
    {QLatin1String("Central Europe Standard Time"), QLatin1String("Europe/Prague")},  /* (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague */
    {QLatin1String("Central European Standard Time"), QLatin1String("Europe/Sarajevo")},      /* (GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb */
    {QLatin1String("Romance Standard Time"), QLatin1String("Europe/Brussels")},       /* (GMT+01:00) Brussels, Copenhagen, Madrid, Paris */
    {QLatin1String("W. Europe Standard Time"), QLatin1String("Europe/Amsterdam")},    /* (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    {QLatin1String("W. Central Africa Standard Time"), QLatin1String("Africa/Bangui")},       /* (GMT+01:00) West Central Africa */
    {QLatin1String("E. Europe Standard Time"), QLatin1String("Europe/Bucharest")},    /* (GMT+02:00) Bucharest */
    {QLatin1String("Egypt Standard Time"), QLatin1String("Africa/Cairo")},            /* (GMT+02:00) Cairo */
    {QLatin1String("FLE Standard Time"), QLatin1String("Europe/Helsinki")},           /* (GMT+02:00) Helsinki, Kiev, Riga, Sofia, Tallinn, Vilnius */
    {QLatin1String("GTB Standard Time"), QLatin1String("Europe/Athens")},             /* (GMT+02:00) Athens, Istanbul, Minsk */
    {QLatin1String("Israel Standard Time"), QLatin1String("Europe/Athens")},          /* (GMT+02:00) Jerusalem */
    {QLatin1String("South Africa Standard Time"), QLatin1String("Africa/Harare")},    /* (GMT+02:00) Harare, Pretoria */
    {QLatin1String("Russian Standard Time"), QLatin1String("Europe/Moscow")},         /* (GMT+03:00) Moscow, St. Petersburg, Volgograd */
    {QLatin1String("Arab Standard Time"), QLatin1String("Asia/Kuwait")},              /* (GMT+03:00) Kuwait, Riyadh */
    {QLatin1String("E. Africa Standard Time"), QLatin1String("Africa/Nairobi")},      /* (GMT+03:00) Nairobi */
    {QLatin1String("Arabic Standard Time"), QLatin1String("Asia/Baghdad")},           /* (GMT+03:00) Baghdad */
    {QLatin1String("Iran Standard Time"), QLatin1String("Asia/Tehran")},              /* (GMT+03:30) Tehran */
    {QLatin1String("Arabian Standard Time"), QLatin1String("Asia/Dubai")},            /* (GMT+04:00) Abu Dhabi, Muscat */
    {QLatin1String("Caucasus Standard Time"), QLatin1String("Asia/Tbilisi")},         /* (GMT+04:00) Baku, Tbilisi, Yerevan */
    {QLatin1String("Transitional Islamic State of Afghanistan Standard Time"), QLatin1String("Asia/Kabul")},  /* (GMT+04:30) Kabul */
    {QLatin1String("Ekaterinburg Standard Time"), QLatin1String("Asia/Yekaterinburg")},       /* (GMT+05:00) Ekaterinburg */
    {QLatin1String("West Asia Standard Time"), QLatin1String("Asia/Karachi")},        /* (GMT+05:00) Islamabad, Karachi, Tashkent */
    {QLatin1String("India Standard Time"), QLatin1String("Asia/Calcutta")},           /* (GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi */
    {QLatin1String("Nepal Standard Time"), QLatin1String("Asia/Calcutta")},           /* (GMT+05:45) Kathmandu */
    {QLatin1String("Central Asia Standard Time"), QLatin1String("Asia/Dhaka")},       /* (GMT+06:00) Astana, Dhaka */
    {QLatin1String("Sri Lanka Standard Time"), QLatin1String("Asia/Colombo")},        /* (GMT+06:00) Sri Jayawardenepura */
    {QLatin1String("N. Central Asia Standard Time"), QLatin1String("Asia/Omsk")},     /* (GMT+06:00) Almaty, Novosibirsk */
    {QLatin1String("Myanmar Standard Time"), QLatin1String("Asia/Rangoon")},          /* (GMT+06:30) Yangon Rangoon */
    {QLatin1String("S.E. Asia Standard Time"), QLatin1String("Asia/Bangkok")},        /* (GMT+07:00) Bangkok, Hanoi, Jakarta */
    {QLatin1String("North Asia Standard Time"), QLatin1String("Asia/Krasnoyarsk")},   /* (GMT+07:00) Krasnoyarsk */
    {QLatin1String("China Standard Time"), QLatin1String("Asia/Shanghai")},           /* (GMT+08:00) Beijing, Chongqing, Hong Kong SAR, Urumqi */
    {QLatin1String("Singapore Standard Time"), QLatin1String("Asia/Singapore")},      /* (GMT+08:00) Kuala Lumpur, Singapore */
    {QLatin1String("Taipei Standard Time"), QLatin1String("Asia/Taipei")},            /* (GMT+08:00) Taipei */
    {QLatin1String("W. Australia Standard Time"), QLatin1String("Australia/Perth")},  /* (GMT+08:00) Perth */
    {QLatin1String("North Asia East Standard Time"), QLatin1String("Asia/Irkutsk")},  /* (GMT+08:00) Irkutsk, Ulaanbaatar */
    {QLatin1String("Korea Standard Time"), QLatin1String("Asia/Seoul")},              /* (GMT+09:00) Seoul */
    {QLatin1String("Tokyo Standard Time"), QLatin1String("Asia/Tokyo")},              /* (GMT+09:00) Osaka, Sapporo, Tokyo */
    {QLatin1String("Yakutsk Standard Time"), QLatin1String("Asia/Yakutsk")},          /* (GMT+09:00) Yakutsk */
    {QLatin1String("A.U.S. Central Standard Time"), QLatin1String("Australia/Darwin")},       /* (GMT+09:30) Darwin */
    {QLatin1String("Cen. Australia Standard Time"), QLatin1String("Australia/Adelaide")},     /* (GMT+09:30) Adelaide */
    {QLatin1String("A.U.S. Eastern Standard Time"), QLatin1String("Australia/Sydney")},       /* (GMT+10:00) Canberra, Melbourne, Sydney */
    {QLatin1String("E. Australia Standard Time"), QLatin1String("Australia/Brisbane")},       /* (GMT+10:00) Brisbane */
    {QLatin1String("Tasmania Standard Time"), QLatin1String("Australia/Hobart")},     /* (GMT+10:00) Hobart */
    {QLatin1String("Vladivostok Standard Time"), QLatin1String("Asia/Vladivostok")},  /* (GMT+10:00) Vladivostok */
    {QLatin1String("West Pacific Standard Time"), QLatin1String("Pacific/Guam")},     /* (GMT+10:00) Guam, Port Moresby */
    {QLatin1String("Central Pacific Standard Time"), QLatin1String("Pacific/Noumea")},        /* (GMT+11:00) Magadan, Solomon Islands, New Caledonia */
    {QLatin1String("Fiji Islands Standard Time"), QLatin1String("Pacific/Fiji")},     /* (GMT+12:00) Fiji Islands, Kamchatka, Marshall Islands */
    {QLatin1String("New Zealand Standard Time"), QLatin1String("Pacific/Auckland")},  /* (GMT+12:00) Auckland, Wellington */
    {QLatin1String("Tonga Standard Time"), QLatin1String("Pacific/Tongatapu")},       /* (GMT+13:00) Nuku'alofa */
    {QLatin1String("Azerbaijan Standard Time"), QLatin1String("America/Argentina/Buenos_Aires")},     /* (GMT-03:00) Buenos Aires */
    {QLatin1String("Middle East Standard Time"), QLatin1String("Asia/Beirut")},       /* (GMT+02:00) Beirut */
    {QLatin1String("Jordan Standard Time"), QLatin1String("Asia/Amman")},             /* (GMT+02:00) Amman */
    {QLatin1String("Central Standard Time (Mexico)"), QLatin1String("America/Mexico_City")},  /* (GMT-06:00) Guadalajara, Mexico City, Monterrey - New */
    {QLatin1String("Mountain Standard Time (Mexico)"), QLatin1String("America/Ojinaga")},     /* (GMT-07:00) Chihuahua, La Paz, Mazatlan - New */
    {QLatin1String("Pacific Standard Time (Mexico)"), QLatin1String("America/Tijuana")},      /* (GMT-08:00) Tijuana, Baja California */
    {QLatin1String("Namibia Standard Time"), QLatin1String("Africa/Windhoek")},       /* (GMT+02:00) Windhoek */
    {QLatin1String("Georgian Standard Time"), QLatin1String("Asia/Tbilisi")},         /* (GMT+03:00) Tbilisi */
    {QLatin1String("Central Brazilian Standard Time"), QLatin1String("America/Manaus")},      /*(GMT-04:00) Manaus */
    {QLatin1String("Montevideo Standard Time"), QLatin1String("America/Montevideo")},        /* (GMT-03:00) Montevideo */
    {QLatin1String("Armenian Standard Time"), QLatin1String("Asia/Yerevan")},         /* (GMT+04:00) Yerevan */
    {QLatin1String("Venezuela Standard Time"), QLatin1String("America/Caracas")},     /* (GMT-04:30) Caracas */
    {QLatin1String("Argentina Standard Time"), QLatin1String("America/Argentina/Buenos_Aires")},      /* (GMT-03:00) Buenos Aires */
    {QLatin1String("Morocco Standard Time"), QLatin1String("Africa/Casablanca")},     /* (GMT) Casablanca */
    {QLatin1String("Pakistan Standard Time"), QLatin1String("Asia/Karachi")},         /* (GMT+05:00) Islamabad, Karachi */
    {QLatin1String("Mauritius Standard Time"), QLatin1String("Indian/Mauritius")},    /* (GMT+04:00) Port Louis */
    {QLatin1String("UTC"), QLatin1String("UTC")},                                     /* (GMT) Coordinated Universal Time */
    {QLatin1String("Paraguay Standard Time"), QLatin1String("America/Asuncion")},     /* (GMT-04:00) Asuncion */
    {QLatin1String("Kamchatka Standard Time"), QLatin1String("Asia/Kamchatka")},      /* (GMT+12:00) Petropavlovsk-Kamchatsky */
};
} // namespace

QString Private::checkAndConverCDOTZID(const QString &tzid, const EventPtr& event)
{
    /* Try to match the @tzid to any valid timezone we know. */
    QTimeZone tz(tzid.toLatin1());
    if (tz.isValid()) {
        /* Yay, @tzid is a valid TZID in Olson format */
        return tzid;
    }

    /* Damn, no match. Parse the iCal and try to find X-MICROSOFT-CDO-TZID
     * property that we can match against the MSCDOTZIDTable */
    KCalendarCore::ICalFormat format;
    /* Use a copy of @event, otherwise it would be deleted when ptr is destroyed */
    KCalendarCore::Incidence::Ptr incidence = event.dynamicCast<KCalendarCore::Incidence>();
    const QString vcard = format.toICalString(incidence);
    const QStringList properties = vcard.split(QLatin1Char('\n'));
    int CDOId = -1;
    for (const QString &property : properties) {
        if (property.startsWith(u"X-MICROSOFT-CDO-TZID")) {
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

        /* *sigh* Some expert in MS assigned the same ID to two different timezones... */
        if (CDOId == 2) {

            /* GMT Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
            if (tzid.contains(QLatin1String("Dublin")) ||
                    tzid.contains(QLatin1String("Edinburgh")) ||
                    tzid.contains(QLatin1String("Lisbon")) ||
                    tzid.contains(QLatin1String("London")))
            {
                return QStringLiteral("Europe/London");
            }

            /* GMT+01:00 Sarajevo, Skopje, Sofija, Vilnius, Warsaw, Zagreb */
            else if (tzid.contains(QLatin1String("Sarajevo")) ||
                     tzid.contains(QLatin1String("Skopje")) ||
                     tzid.contains(QLatin1String("Sofija")) ||
                     tzid.contains(QLatin1String("Vilnius")) ||
                     tzid.contains(QLatin1String("Warsaw")) ||
                     tzid.contains(QLatin1String("Zagreb")))
            {
                return QStringLiteral("Europe/Sarajevo");
            }
        }

        const auto it = MSCDOTZIDTable.find(CDOId);
        if (it != MSCDOTZIDTable.cend()) {
            return it->second;
        }
    }

    /* We failed to map to X-MICROSOFT-CDO-TZID. Let's try mapping the TZID
     * onto the Microsoft Standard Time Zones */
    const auto it = MSSTTZTable.find(tzid);
    if (it != MSSTTZTable.cend()) {
        return it->second;
    }

    /* Fail/ Just return the original TZID and hope Google will accept it
     * (though we know it won't) */
    return tzid;
}

} // namespace CalendarService

} // namespace KGAPI2
