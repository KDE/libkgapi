/*
    libKGoogle - Calendar Service
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


#ifndef SERVICE_CALENDAR_H
#define SERVICE_CALENDAR_H

#include <libkgoogle/kgoogleservice.h>
#include <libkgoogle/libkgoogle_export.h>

#include "qvariant.h"

namespace KGoogle {
  
  class KGoogleObject;
 
  namespace Service {
    
    /**
     * Represents Google Calendar service.
     */
    class LIBKGOOGLE_EXPORT Calendar: public KGoogleService
    {
      public:
	~Calendar() { }

	/**
	 * Implementation of KGoogleService::JSONToObject().
	 */
	KGoogleObject* JSONToObject(const QByteArray& jsonData);
	
	/**
	 * Implementation of KGoogleService::objectToJSON().
	 */
	QByteArray objectToJSON(KGoogleObject* object);
	
	/**
	 * Implementation of KGoogleService::parseJSONFeed().
	 */
	QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);
	
	/**
	 * Implementation of KGoogleService::XMLToObject().
	 */
	KGoogleObject* XMLToObject(const QByteArray& xmlData);
	
	/**
	 * Implementation of KGoogleService::objecttoXML().
	 */
	QByteArray objectToXML(KGoogleObject* object);
	
	/**
	 * Implementation of KGoogleService::parseXMLFeed().
	 */
	QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

	/**
	 * Implementation of KGoogleService::protocolVersion().
	 */
	const QString protocolVersion();
	
	/**
	 * Returns URL for KGoogleRequest::FetchAll requests.
	 */
	static QString fetchAllUrl();
	
	/**
	 * Returns URL for KGoogleRequests::Fetch requests.
	 */
	static QString fetchUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Create requests.
	 */
	static QString createUrl();
	
	/**
	 * Returns URL for KGoogleRequests::Update requests.
	 */
	static QString updateUrl();
	
	/**
	 * Returns URL for KGoogleRequests::Remove requests.
	 */
	static QString removeUrl();
	
	/**
	 * Returns service scope URL
	 */
	static QString scopeUrl();
	
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
	KGoogleObject* JSONToCalendar(const QVariantMap& calendar);
	QVariantMap calendarToJSON(KGoogleObject* calendar);
	QList<KGoogleObject*> parseCalendarJSONFeed(const QVariantList& feed);
	
#if 0
	KGoogleObject* XMLToCalendar(const QDomElement& calendar);
	QDomElement calendarToXML(const KGoogleObject *calendar);
	QList<KGoogleObject*> parseCalendarXMLFeed(const QDomElement& feed);
#endif
	
	KGoogleObject* JSONToEvent(const QVariantMap& event);
	QVariantMap eventToJSON(KGoogleObject *event);
	QList<KGoogleObject*> parseEventJSONFeed(const QVariantList& feed);
	
#if 0
	KGoogleObject* XMLToEvent(const QDomElement& event);
	QDomElement eventToXML(const KGoogleObject *event);
	QList<KGoogleObject*> parseEventXMLFeed(const QDomElement& feed);
#endif
    };
    
  } // namespace Service

} // namespace KGoogle

#endif // SERVICE_CALENDAR_H
