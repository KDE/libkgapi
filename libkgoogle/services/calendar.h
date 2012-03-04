/*
    libKGoogle - Services - Calendar
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LIBKGOOGLE_SERVICES_CALENDAR_H
#define LIBKGOOGLE_SERVICES_CALENDAR_H

#include <libkgoogle/service.h>
#include <libkgoogle/libkgoogle_export.h>

#ifdef WITH_KCAL
#include <KDE/KCal/IncidenceBase>
using namespace KCal;
#else
#include <KDE/KCalCore/IncidenceBase>
using namespace KCalCore;
#endif

namespace KGoogle
{

class Object;

namespace Services
{

class CalendarPrivate;

/**
* Represents Google Calendar service.
*/
class LIBKGOOGLE_EXPORT Calendar: public KGoogle::Service
{
  public:
    static QUrl ScopeUrl;

    /**
     * Implementation of KGoogle::Service::JSONToObject().
     */
    KGoogle::Object* JSONToObject(const QByteArray& jsonData);

    /**
     * Implementation of KGoogle::Service::objectToJSON().
     */
    QByteArray objectToJSON(KGoogle::Object* object);

    /**
     * Implementation of KGoogle::Service::parseJSONFeed().
     */
    QList< KGoogle::Object* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);

    /**
     * Implementation of KGoogle::Service::XMLToObject().
     */
    KGoogle::Object* XMLToObject(const QByteArray& xmlData);

    /**
     * Implementation of KGoogle::Service::objecttoXML().
     */
    QByteArray objectToXML(KGoogle::Object* object);

    /**
     * Implementation of KGoogle::Service::parseXMLFeed().
     */
    QList< KGoogle::Object* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

    /**
     * Implementation of KGoogle::Service::protocolVersion().
     */
    QString protocolVersion() const;

    /**
     * Returns URL for fetching calendars list.
     */
    static QUrl fetchCalendarsUrl();

    /**
     * Returns URL for fetching single calendar.
     *
     * @param calendarID calendar ID
     */
    static QUrl fetchCalendarUrl(const QString &calendarID);

    /**
     * Returns URL for updating existing calendar.
     *
     * @param user "default" or user@gmail.com
     * @param calendarID ID of calendar to modify
     */
    static QUrl updateCalendarUrl(const QString &calendarID);

    /**
     * Returns URL for creating a new calendar.
     */
    static QUrl createCalendarUrl();

    /**
     * Returns URL for removing an existing calendar.
     *
     * @param calendarID ID of calendar to remove
     */
    static QUrl removeCalendarUrl(const QString &calendarID);

    /**
     * Returns URL for fetching all events from a specific calendar
     *
     * @param calendarID ID of calendar from which to fetch events
     */
    static QUrl fetchEventsUrl(const QString &calendarID);

    /**
     * Returns URL for fetching a single event from a specific calendar.
     *
     * @param calendarID ID of calendar from which to fetch the event
     * @param eventID ID of event to fetch
     */
    static QUrl fetchEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * Returns URL for updating a single event
     *
     * @param calendarID ID of calendar in which the event is
     * @param eventID ID of event to update
     */
    static QUrl updateEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * Returns URL creating new events.
     *
     * @param calendarID ID of calendar in which to create the event
     */
    static QUrl createEventUrl(const QString &calendarID);

    /**
     * Returns URL for removing events
     *
     * @param calendarID ID of parent calendar
     * @param eventID ID of event to remove.
     */
    static QUrl removeEventUrl(const QString &calendarID, const QString &eventID);

    /**
     * Returns URL for moving event between calendars.
     *
     * @param sourceCalendar ID of calendar from which to remove the event
     * @param destCalendar ID of calendar to which to move the even
     * @param eventID ID of event in the \p sourceCalendar to move
     */
    static QUrl moveEventUrl(const QString &sourceCalendar, const QString &destCalendar, const QString &eventID);

    /**
     * Returns service scope URL
     *
     * https://www.google.com/calendar/feeds/
     */
    const QUrl& scopeUrl() const;

    /**
     * Returns wheter service supports reading data in JSON format.
     *
     * @param urlParam Returns value of "alt" query. Usually is
     * "json" or "jsonc". When service does not support reading JSON
     * data, the value remains unchanged.
     */
    static bool supportsJSONRead(QString* urlParam);

    /**
     * Returns wheter service supports writing data in JSON format.
     *
     * @param urlParam Returns value of "alt" query. Usually is
     * "json" or "jsonc". When service does not support writing JSON
     * data, the value remains unchanged.
     */
    static bool supportsJSONWrite(QString* urlParam);

  private:
    CalendarPrivate *d_ptr;
    Q_DECLARE_PRIVATE(Calendar);
};

} // namespace Services

} // namespace KGoogle

#endif // LIBKGOOGLE_SERVICES_CALENDAR_H
