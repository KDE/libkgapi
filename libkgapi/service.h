/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_SERVICE_H
#define LIBKGAPI_SERVICE_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include <libkgapi/common.h>
#include <libkgapi/libkgapi_export.h>

namespace KGAPI
{

class Object;

/**
 * A base class for all services.
 *
 * Service is a class that provides additional information
 * about a Google service and implements parsers for objects
 * from service.
 */
class LIBKGAPI_EXPORT Service
{

  public:
    virtual ~Service() { }

    /**
     * Parses raw \p jsonData to a KGAPI::Object.
     */
    virtual KGAPI::Object* JSONToObject(const QByteArray& jsonData) = 0;

    /**
     * Serializes a KGAPIObject to raw JSON data.
     */
    virtual QByteArray objectToJSON(KGAPI::Object* object) = 0;

    /**
     * Parses raw JSON feed data into list of KGAPI::Object objects.
     */
    virtual QList< KGAPI::Object* > parseJSONFeed(const QByteArray& jsonFeed, KGAPI::FeedData& feedData) = 0;

    /**
     * Parsers raw \p xmlData into a KGAPI::Object.
     */
    virtual KGAPI::Object* XMLToObject(const QByteArray& xmlData) = 0;

    /**
     * Serializes a KGAPIObject to raw XML data.
     */
    virtual QByteArray objectToXML(KGAPI::Object* object) = 0;

    /**
     * Parses raw JSON feed data into list of KGAPI::Object objects.
     */
    virtual QList< KGAPI::Object* > parseXMLFeed(const QByteArray& xmlFeed, KGAPI::FeedData& feedData) = 0;

    /**
     * Returns version of the services protocol.
     */
    virtual QString protocolVersion() const = 0;

    /**
     * Returns service scope URL.
     */
    virtual const QUrl& scopeUrl() const = 0;

};

} // namespace KGAPI

#endif // LIBKGAPI_SERVICE_H
