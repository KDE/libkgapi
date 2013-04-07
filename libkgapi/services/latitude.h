/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef LIBKGAPI_SERVICES_LATITUDE_H
#define LIBKGAPI_SERVICES_LATITUDE_H

#include <libkgapi/service.h>
#include <libkgapi/libkgapi_export.h>

#include <QVariantMap>

namespace KGAPI
{
class Object;

namespace Services
{

class LIBKGAPI_EXPORT_DEPRECATED Latitude : public KGAPI::Service
{
  public:
    enum Granularity {
        City, Best
    };

    static QUrl ScopeUrl;

    /**
     * Implementation of KGAPI::Service::name().
     */
    static QString serviceName();

    /**
     * Implementation of KGAPI::Service::JSONToObject().
     */
    KGAPI::Object* JSONToObject(const QByteArray& jsonData);

    /**
     * Implementation of KGAPI::Service::ObjectTOJSON().
     */
    QByteArray objectToJSON(KGAPI::Object* object);

    /**
     * Implementation of KGAPI::Service::parseJSONFeed().
     */
    QList< KGAPI::Object* > parseJSONFeed(const QByteArray& jsonFeed, KGAPI::FeedData& feedData);

    /**
     * Implementation of KGAPI::Service::XMLToObject().
     */
    KGAPI::Object* XMLToObject(const QByteArray& xmlData);

    /**
     * Implementation of KGAPI::Service::objectToXML().
     */
    QByteArray objectToXML(KGAPI::Object* object);

    /**
     * Implementation of KGAPI::Service::parseXMLFeed().
     */
    QList< KGAPI::Object* > parseXMLFeed(const QByteArray& xmlFeed, KGAPI::FeedData& feedData);

    /**
     * Implementation of KGAPI::Service::protocolVersion().
     */
    QString protocolVersion() const;

    /**
     * Returns service scope URL for Google Latitude service
     */
    QUrl scopeUrl() const;

    /**
     *  Returns URL for KGAPI::Request::Fetch requests.
     */
    static QUrl retrieveCurrentLocationUrl(const Latitude::Granularity granularity = City);

    /**
     *  Returns URL for KGAPI::Request::Remove requests.
     */
    static QUrl deleteCurrentLocationUrl();

    /**
     *  Returns URL for KGAPI::Request::Create requests.
     */
    static QUrl insertCurrentLocationUrl();

    /**
     * Returns URL for KGAPI::Request::FetchAll requests.
     *
     * @param granularity Precision of the coordinates
     * @param maxResults Maximum amount of results to return
     * @param maxTime Maximum timestamp since epoch (in ms)
     * @param minTime Minimum timestamp since epoch (in ms)
     */
    static QUrl locationHistoryUrl(const Latitude::Granularity granularity, const int maxResults = 0,
                                   const qlonglong maxTime = 0, const qlonglong minTime = 0);

    /**
     * Returns URL for KGAPI::Request::Fetch requests.
     */
    static QUrl retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity = City);

    /**
     * Returns URL for KGAPI::Request::Create requests.
     */
    static QUrl insertLocationUrl();

    /**
     * Returns URL for KGAPI::Request::Remove requests.
     */
    static QUrl deleteLocationUrl(const qlonglong id);

  private:
    //krazy:exclude=dpointer

};

} // namespace Services

} // namespace KGAPI

#endif // LIBGKAPI_SERVICES_LATITUDE_H
