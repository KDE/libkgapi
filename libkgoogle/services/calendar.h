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

#include "qvariant.h"

namespace KGoogle {

  class Object;

  namespace Services {

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
	 * Returns URL for KGoogle::Request::FetchAll requests.
         *
         * @param user "default" or user@gmail.com
         * @param visibility "allcalendars" for list of calendars, "private" for list of events
	 */
	static QUrl fetchAllUrl(const QString &user, const QString &visibility);

	/**
	 * Returns URL for KGoogle::Requests::Fetch requests.
         *
         * @param user "default" or user@gmail.com
         * @param visibility "allcalendars" for calendar, "private" for event
         * @param objID event ID or calendar ID (depending on \p visibility)
	 */
	static QUrl fetchUrl(const QString &user, const QString &visibility, const QString objID);

	/**
	 * Returns URL for KGoogle::Request::Create requests.
         *
         * @param user "default" or user@gmail.com
         * @param visibility "allcalendars" when creating a calendar, "private" when creating an event
	 */
	static QUrl createUrl(const QString &user, const QString &visibility);

	/**
	 * Returns URL for KGoogle::Requests::Update requests.
         *
         * @param user "default" or user@gmail.com
         * @param visibility "allcalendars" when updating a calendar, "private" when updating an event
         * @param objID event ID or calendar ID (depending on \p visibility)
	 */
	static QUrl updateUrl(const QString &user, const QString &visibility, const QString &objID);

	/**
	 * Returns URL for KGoogle::Requests::Remove requests.
         *
         * @param user "default" or user@gmail.com
         * @param visibility "allcalendars" when removing a calendar, "private" when removing an event
         * @param objID event ID of calendar ID (depending on \p visiblity)
	 */
	static QUrl removeUrl(const QString &user, const QString &visibility, const QString &objID);

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
	KDateTime parseRecurrenceDT(const QString &dt, bool *allday);

	KGoogle::Object* JSONToCalendar(const QVariantMap& calendar);
	QVariantMap calendarToJSON(KGoogle::Object* calendar);
	QList<KGoogle::Object*> parseCalendarJSONFeed(const QVariantList& feed);

#if 0
	KGoogleObject* XMLToCalendar(const QDomElement& calendar);
	QDomElement calendarToXML(const KGoogleObject *calendar);
	QList<KGoogleObject*> parseCalendarXMLFeed(const QDomElement& feed);
#endif

	KGoogle::Object* JSONToEvent(const QVariantMap& event);
	QVariantMap eventToJSON(KGoogle::Object *event);
	QList<KGoogle::Object*> parseEventJSONFeed(const QVariantList& feed);

#if 0
	KGoogleObject* XMLToEvent(const QDomElement& event);
	QDomElement eventToXML(const KGoogleObject *event);
	QList<KGoogleObject*> parseEventXMLFeed(const QDomElement& feed);
#endif
    };

  } // namespace Services

} // namespace KGoogle

#endif // LIBKGOOGLE_SERVICES_CALENDAR_H
