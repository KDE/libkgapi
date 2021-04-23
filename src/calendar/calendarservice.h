/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"
#include "enums.h"
#include "kgapicalendar_export.h"

#include <QFlags>

class QNetworkRequest;

namespace KGAPI2
{

/**
 * @brief Additional methods for implementing support for Google Calendar service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace CalendarService
{
    enum class EventSerializeFlag {
        Default     =   0,
        NoID        =   1 << 0
    };
    using EventSerializeFlags = QFlags<EventSerializeFlag>;

    /**
     * @brief Preparse a QNetworkRequest for given URL
     *
     * @param url
     */
    KGAPICALENDAR_EXPORT QNetworkRequest prepareRequest(const QUrl &url);

    /**
     * @brief Parses calendar JSON data into Calendar object
     *
     * @param jsonData
     */
    KGAPICALENDAR_EXPORT CalendarPtr JSONToCalendar(const QByteArray& jsonData);

    /**
     * @brief Serializes calendar into JSON
     *
     * @param calendar
     */
    KGAPICALENDAR_EXPORT QByteArray calendarToJSON(const CalendarPtr& calendar);

    /**
     * @brief Parses JSON feed into list of Calendars
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information about
     *                 the feed, including URL for next page (if any)
     */
    KGAPICALENDAR_EXPORT ObjectsList parseCalendarJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Parses event JSON into Event object
     *
     * @param jsonData
     */
    KGAPICALENDAR_EXPORT EventPtr JSONToEvent(const QByteArray& jsonData);

    /**
     * @brief Serializes Event into JSON
     *
     * @param event
     */
    KGAPICALENDAR_EXPORT QByteArray eventToJSON(const EventPtr& event, EventSerializeFlags flags = EventSerializeFlag::Default);

    /**
     * @brief Parses JSON feed into list of Events
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information about
     *                  the feed, including URL for next page (if any)
     */
    KGAPICALENDAR_EXPORT ObjectsList parseEventJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Supported API version
     */
    KGAPICALENDAR_EXPORT QString APIVersion();

    /**
     * @brief Returns URL for fetching calendars list.
     */
    KGAPICALENDAR_EXPORT QUrl fetchCalendarsUrl();

    /**
     * @brief Returns URL for fetching single calendar.
     *
     * @param calendarID calendar ID
     */
    KGAPICALENDAR_EXPORT QUrl fetchCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for updating existing calendar.
     *
     * @param calendarID ID of calendar to modify
     */
    KGAPICALENDAR_EXPORT QUrl updateCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for creating a new calendar.
     */
    KGAPICALENDAR_EXPORT QUrl createCalendarUrl();

    /**
     * @brief Returns URL for removing an existing calendar.
     *
     * @param calendarID ID of calendar to remove
     */
    KGAPICALENDAR_EXPORT QUrl removeCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for fetching all events from a specific calendar
     *
     * @param calendarID ID of calendar from which to fetch events
     */
    KGAPICALENDAR_EXPORT QUrl fetchEventsUrl(const QString &calendarID);

    /**
     * @brief Returns URL for fetching a single event from a specific calendar.
     *
     * @param calendarID ID of calendar from which to fetch the event
     * @param eventID ID of event to fetch
     */
    KGAPICALENDAR_EXPORT QUrl fetchEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * @brief Returns URL for updating a single event
     *
     * @param calendarID ID of calendar in which the event is
     * @param eventID ID of event to update
     * @param updatesPolicy Whether to send notification to participants
     */
    KGAPICALENDAR_EXPORT QUrl updateEventUrl(const QString &calendarID, const QString &eventID,
                                             SendUpdatesPolicy updatesPolicy);

    /**
     * @brief Returns URL creating new events.
     *
     * @param calendarID ID of calendar in which to create the event
     * @param updatesPolicy Whether to send notification to participants
     */
    KGAPICALENDAR_EXPORT QUrl createEventUrl(const QString &calendarID, SendUpdatesPolicy updatesPolicy);
    
    /**
     * @brief Returns URL importing private copies of existing events.
     *
     * @param calendarID ID of calendar in which to create the event
     * @param updatesPolicy Whether to send notification to participants
     */
    KGAPICALENDAR_EXPORT QUrl importEventUrl(const QString &calendarID, SendUpdatesPolicy updatesPolicy);

    /**
     * @brief Returns URL for removing events
     *
     * @param calendarID ID of parent calendar
     * @param eventID ID of event to remove.
     */
    KGAPICALENDAR_EXPORT QUrl removeEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * @brief Returns URL for moving event between calendars.
     *
     * @param sourceCalendar ID of calendar from which to remove the event
     * @param destCalendar ID of calendar to which to move the even
     * @param eventID ID of event in the \p sourceCalendar to move
     */
    KGAPICALENDAR_EXPORT QUrl moveEventUrl(const QString &sourceCalendar, const QString &destCalendar, const QString &eventID);

    /**
     * @brief Returns URL for freebusy queries.
     */
    KGAPICALENDAR_EXPORT QUrl freeBusyQueryUrl();

} // namespace CalendarService

} // namespace KGAPI

Q_DECLARE_OPERATORS_FOR_FLAGS(KGAPI2::CalendarService::EventSerializeFlags)

