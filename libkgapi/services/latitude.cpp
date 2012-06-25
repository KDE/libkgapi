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


#include "latitude.h"
#include "accessmanager.h"
#include "objects/location.h"

#include <KDateTime>
#include <kurl.h>

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDebug>

using namespace KGAPI;
using namespace Services;

QUrl Latitude::ScopeUrl("https://www.googleapis.com/auth/latitude.all.best");

Object * Latitude::JSONToObject(const QByteArray & jsonData)
{
    QJson::Parser parser;
    QVariantMap data;
    bool ok;
    
    data = parser.parse(jsonData, &ok).toMap();
    
    if (!ok) {
        throw Exception::InvalidResponse();
        return 0;
    }
    
    QVariantMap info = data["data"].toMap();
    
    return parseLocation(info);
}

QByteArray Latitude::objectToJSON(Object * object)
{
    QVariantMap map, output;
    Objects::Location *location = static_cast<Objects::Location*>(object);
    
    map["kind"] = "latitude#location";
    if (location->timestamp() != 0)
        map["timestampMs"] = location->timestamp();
    map["latitude"] = QString::number(location->latitude());
    map["longitude"] = QString::number(location->longitude());
    
    if (location->accuracy() != -1)
        map["accuracy"] = location->accuracy();
    if (location->speed() != -1)
        map["speed"] = location->speed();
    if (location->heading() != -1)
        map["heading"] = location->heading();
    if (location->altitude() != -1)
        map["altitude"] = location->altitude();
    if (location->altitudeAccuracy() != 0)
        map["altitudeAccuracy"] = location->altitudeAccuracy();
    
    output["data"] = map;
    
    QJson::Serializer serializer;
    return serializer.serialize(output);
}

QList< Object * > Latitude::parseJSONFeed(const QByteArray & jsonFeed, FeedData & feedData)
{    
    Q_UNUSED(feedData);
    
    QList<KGAPI::Object*> output;
    QJson::Parser parser;
    
    QVariantMap map = parser.parse(jsonFeed).toMap();
    QVariantMap data = map["data"].toMap();
    
    Q_FOREACH(const QVariant &c, data["items"].toList()) {
        QVariantMap location = c.toMap();
        output << parseLocation(location);   
    }

    return output;;
}

QByteArray Latitude::objectToXML(Object * object)
{
    Q_UNUSED(object);

    return QByteArray();
}

Object * Latitude::XMLToObject(const QByteArray & xmlData)
{
    Q_UNUSED(xmlData)

    return 0;
}

QList< Object * > Latitude::parseXMLFeed(const QByteArray & xmlFeed, FeedData & feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object * >();
}

Object * Latitude::parseLocation(const QVariantMap map)
{
    Objects::Location * object = new Objects::Location();
    
    if (map.contains("timestampMs"))
        object->setTimestamp(map["timestampMs"].toULongLong());
    if (map.contains("latitude"))
        object->setLatitude(map["latitude"].toFloat());
    if (map.contains("longitude"))
        object->setLongitude(map["longitude"].toFloat());
    if (map.contains("accuracy"))
        object->setAccuracy(map["accuracy"].toInt());
    if (map.contains("speed"))
        object->setSpeed(map["speed"].toInt());
    if (map.contains("heading"))
        object->setHeading(map["heading"].toInt());
    if (map.contains("altitude"))
        object->setAltitude(map["altitude"].toInt());
    if (map.contains("altitudeAccuracy"))
        object->setAltitudeAccuracy(map["altitudeAccuracy"].toInt());
    
    return object;
}


QString Latitude::protocolVersion() const
{
    return "1";
}

const QUrl & Latitude::scopeUrl() const
{
    return Latitude::ScopeUrl;
}

QUrl Latitude::retrieveCurrentLocationUrl(const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/currentLocation");
    
    if (granularity == City)
        url.addQueryItem("granularity", "city");
    else if (granularity == Best)
        url.addQueryItem("granularity", "best");
        
    return QUrl(url);
}

QUrl Latitude::deleteCurrentLocationUrl()
{
    return QUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl Latitude::insertCurrentLocationUrl()
{
    return QUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl Latitude::locationHistoryUrl(const Latitude::Granularity granularity, 
                               const int maxResults, const int maxTime, const int minTime)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location");
    
    if (granularity == City)
        url.addQueryItem("granularity", "city");
    else if (granularity == Best)
        url.addQueryItem("granularity", "best");
    
    if (maxResults > 0)
        url.addQueryItem("max-results",QString::number(maxResults));
    if (maxTime > 0 && maxTime > minTime)
        url.addQueryItem("max-time", QString::number(maxTime));
    if (minTime > 0 && minTime < maxTime)
        url.addQueryItem("min-time", QString::number(minTime));
    
    return url;
}

QUrl Latitude::retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));
    
     if (granularity == City)
        url.addQueryItem("granularity", "city");
     else if (granularity == Best)
        url.addQueryItem("granularity", "best");
    
    return url;
}

QUrl Latitude::insertLocationUrl()
{
    return QUrl("https://www.googleapis.com/latitude/v1/location");
}

QUrl Latitude::deleteLocationUrl(const qlonglong id)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));
    
    return url;
}
