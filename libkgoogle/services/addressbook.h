/*
    libKGoogle - Services - Addressbook
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


#ifndef LIBKGOOGLE_SERVICES_ADDRESSBOOK_H
#define LIBKGOOGLE_SERVICES_ADDRESSBOOK_H

#include <libkgoogle/service.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qmetatype.h>

namespace KGoogle {

  class Object;

  namespace Services {

    /**
     * Represents Google Contacts service.
     */
    class LIBKGOOGLE_EXPORT Addressbook: public KGoogle::Service
    {
      public:
	~Addressbook();

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
	 * Implementation of KGoogle::Service::objectToXML().
	 */
	QByteArray objectToXML(KGoogle::Object* object);

	/**
	 * Implementation of KGoogle::Service::parseXMLFeed().
	 */
	QList< KGoogle::Object* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

	/**
	 * Implementation of KGoogle::Service::protocolVersion().
	 */
	const QString protocolVersion();

	/**
	 * Returns scope URL for Google Calendar service.
         *
         * https://www.google.com/m8/feeds/
	 */
	static QString scopeUrl();

	/**
	 * Returns URL for KGoogle::Request::Create requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full
         * %1 = user ("default" or user@gmail.com)
	 */
	static QString createUrl();

	/**
	 * Returns URL for KGoogle::Request::FetchAll requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full
         * %1 = user ("default" or user@gmail.com)
	 */
	static QString fetchAllUrl();

	/**
	 * Returns URL for KGoogle::Request::Fetch requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full/%2
         * %1 = user ("default" or user@gmail.com)
         * %2 = contact ID
	 */
	static QString fetchUrl();

	/**
	 * Returns URL for KGoogle::Request::Update requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full/%2
         * %1 = user ("default" or user@gmail.com)
         * %2 = contact ID
	 */
	static QString updateUrl();

	/**
	 * Returns URL for KGoogle::Request::Remove requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full/%2
         * %1 = user ("default" or user@gmail.com)
         * %2 = contact ID
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

  } // namespace Services

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Services::Addressbook)

#endif // LIBKGOOGLE_SERVICES_ADDRESSBOOK_H
