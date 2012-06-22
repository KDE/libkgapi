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
    
    QVariantMap location = data["data"].toMap();
    
    Objects::Location * object = new Objects::Location();
    
    if (location.contains("timestampMs"))
        object->setTimestamp(location["timestampMs"].toULongLong());
    if (location.contains("latitude"))
        object->setLatitude(location["latitude"].toFloat());
    if (location.contains("longitude"))
        object->setLongitude(location["longitude"].toFloat());
    if (location.contains("accuracy"))
        object->setAccuracy(location["accuracy"].toInt());
    if (location.contains("speed"))
        object->setSpeed(location["speed"].toInt());
    if (location.contains("heading"))
        object->setHeading(location["heading"].toInt());
    if (location.contains("altitude"))
        object->setAltitude(location["altitude"].toInt());
    if (location.contains("altitudeAccuracy"))
        object->setAltitudeAccuracy(location["altitudeAccuracy"].toInt());
    
    return object;
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
    Q_UNUSED(jsonFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object *>();
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
    QString url = "https://www.googleapis.com/latitude/v1/currentLocation";
    
    if (granularity == City)
        url += "?granularity=city";
    else if (granularity == Best)
        url += "?granularity=best";
        
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
