/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef SERVICE_ADDRESSBOOK_H
#define SERVICE_ADDRESSBOOK_H

#include <libkgoogle/kgoogleservice.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qmetatype.h>

namespace KGoogle {
  
  class KGoogleObject;
 
  namespace Service {

    /**
     * Represents Google Contacts service.
     */
    class LIBKGOOGLE_EXPORT Addressbook: public KGoogleService
    {
      public:
	~Addressbook();
	
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
	 * Implementation of KGoogleService::objectToXML().
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
	 * Returns scope URL for Google Calendar service.
	 */
	static QString scopeUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Create requests.
	 */
	static QString createUrl();
	
	/**
	 * Returns URL for KGoogleRequest::FetchAll requests.
	 */
	static QString fetchAllUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Fetch requests.
	 */
	static QString fetchUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Update requests.
	 */
	static QString updateUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Remove requests.
	 */
	static QString removeUrl();
	
	/**
	 * Returns wheter service supports reading data in JSON format.
	 * 
	 * @param urlParam Returns a value of "alt" URL query. Usually is 
	 * "json" or "jsonc". When service does not support reading JSON
	 * data, the value remains unchanged.
	 */
	static bool supportsJSONRead(QString* urlParam);
	
	/**
	 * Returns wheter service supports writing data in JSON format.
	 * 
	 * @param urlParam Returns a value of "alt" URL query. Usually is
	 * "json" or "jsonc". When service does not support writing JSON
	 * data, the value remains unchanged.
	 */
	static bool supportsJSONWrite(QString* urlParam);
    };
    
  } // namespace Service
  
} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Service::Addressbook)

#endif // SERVICE_ADDRESSBOOK_H
