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

#ifndef LIBKGAPI_SERVICES_ACCOUNTINFO_H
#define LIBKGAPI_SERVICES_ACCOUNTINFO_H

#include <libkgapi/service.h>
#include <libkgapi/libkgapi_export.h>

namespace KGAPI
{

class Object;

namespace Services
{

class LIBKGAPI_EXPORT_DEPRECATED AccountInfo : public KGAPI::Service
{
  public:
    static QUrl ScopeUrl;

    /**
     * Implements KGAPI::Service::JSONToObject().
     */
    KGAPI::Object *JSONToObject(const QByteArray &jsonData);

    /**
     * Service version 1 is supported now.
     */
    QString protocolVersion() const;

    /**
     * The AccountInfo provides read-only access, so serializer is not implemented.
     *
     * This method does nothing.
     */
    QByteArray objectToJSON(KGAPI::Object *object);

    /**
     * Returns scope URL for AccountInfo service.
     */
    QUrl scopeUrl() const;

    /**
     * Returns KGAPI::Request::Fetch URL.
     *
     * https://www.googleapis.com/oauth2/v1/userinfo
     */
    static QUrl fetchUrl();

    /**
     * Implements KGAPI::Service::name()
     */
    static QString serviceName();

    /**
     * The AccountInfo always returns information about single account, never feed.
     *
     * This method does nothing.
     */
    QList< KGAPI::Object * > parseJSONFeed(const QByteArray &jsonFeed, KGAPI::FeedData& feedData);

    /**
     * The AccountInfo service supports only JSON format.
     *
     * This method does nothing.
     */
    QList< KGAPI::Object * > parseXMLFeed(const QByteArray &xmlFeed, KGAPI::FeedData& feedData);

    /**
     * The AccountInfo service supports only JSON format.
     *
     * This method does nothing.
     */
    QByteArray objectToXML(KGAPI::Object *object);

    /**
     * The AccountInfo service supports only JSON format.
     *
     * This method does nothing.
     */
    KGAPI::Object *XMLToObject(const QByteArray &xmlData);

  private:
      //krazy:exclude=dpointer
};

} /* namespace Services */

} /* namespace KGAPI */

#endif // LIBKGAPI_SERVICES_ACCOUNTINFO_H
