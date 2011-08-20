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

#include "kgoogleservice.h"
#include "qvariant.h"

#include "libkgoogle_export.h"

namespace KGoogle {
  
  class KGoogleObject;
 
  namespace Service {
    
    class LIBKGOOGLE_EXPORT Calendar: public KGoogleService
    {
      public:
	~Calendar() { }

	KGoogleObject* JSONToObject(const QByteArray& jsonData);
	QByteArray objectToJSON(KGoogleObject* object);
	QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);
	
	KGoogleObject* XMLToObject(const QByteArray& xmlData);
	QByteArray objectToXML(KGoogleObject* object);
	QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

	const QString protocolVersion();
	
	static QString fetchAllUrl();
	static QString fetchUrl();
	static QString createUrl();
	static QString updateUrl();
	static QString removeUrl();
	static QString scopeUrl();
	
	static bool supportsJSONRead(QString* urlParam);
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
