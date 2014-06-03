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

#ifndef LIBKGAPI2_CALENDARSERVICE_H
#define LIBKGAPI2_CALENDARSERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{

/**
 * @brief Additional methods for implementing support for Google Calendar service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace CalendarService
{

    /**
     * @brief Parses calendar JSON data into Calendar object
     *
     * @param JsonData
     */
    LIBKGAPI2_EXPORT CalendarPtr JSONToCalendar(const QByteArray& jsonData);

    /**
     * @brief Serializes calendar into JSON
     *
     * @param calendar
     */
    LIBKGAPI2_EXPORT QByteArray calendarToJSON(const CalendarPtr& calendar);

    /**
     * @brief Parses JSON feed into list of Calendars
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information about
     *                 the feed, including URL for next page (if any)
     */
    LIBKGAPI2_EXPORT ObjectsList parseCalendarJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Parses event JSON into Event object
     *
     * @param jsonData
     */
    LIBKGAPI2_EXPORT EventPtr JSONToEvent(const QByteArray& jsonData);

    /**
     * @brief Serializes Event into JSON
     *
     * @param event
     */
    LIBKGAPI2_EXPORT QByteArray eventToJSON(const EventPtr& event);

    /**
     * @brief Parses JSON feed into list of Events
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information about
     *                  the feed, including URL for next page (if any)
     */
    LIBKGAPI2_EXPORT ObjectsList parseEventJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Supported API verstion
     */
    LIBKGAPI2_EXPORT QString APIVersion();

    /**
     * @brief Returns URL for fetching calendars list.
     */
    LIBKGAPI2_EXPORT QUrl fetchCalendarsUrl();

    /**
     * @brief Returns URL for fetching single calendar.
     *
     * @param calendarID calendar ID
     */
    LIBKGAPI2_EXPORT QUrl fetchCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for updating existing calendar.
     *
     * @param calendarID ID of calendar to modify
     */
    LIBKGAPI2_EXPORT QUrl updateCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for creating a new calendar.
     */
    LIBKGAPI2_EXPORT QUrl createCalendarUrl();

    /**
     * @brief Returns URL for removing an existing calendar.
     *
     * @param calendarID ID of calendar to remove
     */
    LIBKGAPI2_EXPORT QUrl removeCalendarUrl(const QString &calendarID);

    /**
     * @brief Returns URL for fetching all events from a specific calendar
     *
     * @param calendarID ID of calendar from which to fetch events
     */
    LIBKGAPI2_EXPORT QUrl fetchEventsUrl(const QString &calendarID);

    /**
     * @brief Returns URL for fetching a single event from a specific calendar.
     *
     * @param calendarID ID of calendar from which to fetch the event
     * @param eventID ID of event to fetch
     */
    LIBKGAPI2_EXPORT QUrl fetchEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * @brief Returns URL for updating a single event
     *
     * @param calendarID ID of calendar in which the event is
     * @param eventID ID of event to update
     */
    LIBKGAPI2_EXPORT QUrl updateEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * @brief Returns URL creating new events.
     *
     * @param calendarID ID of calendar in which to create the event
     */
    LIBKGAPI2_EXPORT QUrl createEventUrl(const QString &calendarID);

    /**
     * @brief Returns URL for removing events
     *
     * @param calendarID ID of parent calendar
     * @param eventID ID of event to remove.
     */
    LIBKGAPI2_EXPORT QUrl removeEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * @brief Returns URL for moving event between calendars.
     *
     * @param sourceCalendar ID of calendar from which to remove the event
     * @param destCalendar ID of calendar to which to move the even
     * @param eventID ID of event in the \p sourceCalendar to move
     */
    LIBKGAPI2_EXPORT QUrl moveEventUrl(const QString &sourceCalendar, const QString &destCalendar, const QString &eventID);

} // namespace CalendarService

} // namespace KGAPI

#endif // LIBKGAPI2_CALENDARSERVICE_H
