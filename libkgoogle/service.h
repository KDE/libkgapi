/*
    libKGoogle - Service
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


#ifndef LIBKGOOGLE_SERVICE_H
#define LIBKGOOGLE_SERVICE_H

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include <libkgoogle/common.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle
{

class Object;

/**
 * A base class for all services.
 *
 * Service is a class that provides additional informations
 * about a Google service and implements parsers for objects
 * from service.
 */
class LIBKGOOGLE_EXPORT Service
{

  public:
    virtual ~Service() { }

    /**
     * Parses raw \p jsonData to a KGoogleObject.
     */
    virtual KGoogle::Object* JSONToObject(const QByteArray& jsonData) = 0;

    /**
     * Serializes a KGoogleObject to raw JSON data.
     */
    virtual QByteArray objectToJSON(KGoogle::Object* object) = 0;

    /**
     * Parses raw JSON feed data into list of KGoogleObject objects.
     */
    virtual QList< KGoogle::Object* > parseJSONFeed(const QByteArray& jsonFeed, KGoogle::FeedData& feedData) = 0;

    /**
     * Parsers raw \p xmlData into a KGoogleObject.
     */
    virtual KGoogle::Object* XMLToObject(const QByteArray& xmlData) = 0;

    /**
     * Serializes a KGoogleObject to raw XML data.
     */
    virtual QByteArray objectToXML(KGoogle::Object* object) = 0;

    /**
     * Parses raw JSON feed data into list of KGoogleObject objects.
     */
    virtual QList< KGoogle::Object* > parseXMLFeed(const QByteArray& xmlFeed, KGoogle::FeedData& feedData) = 0;

    /**
     * Returns version of the services protocol.
     */
    virtual QString protocolVersion() const = 0;

    /**
     * Returns service scope URL.
     */
    virtual const QUrl& scopeUrl() const = 0;

};

} // namespace KGoogle

#endif // LIBKGOOGLE_SERVICE_H
