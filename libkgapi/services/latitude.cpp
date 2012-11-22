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

QUrl Latitude::ScopeUrl(QLatin1String("https://www.googleapis.com/auth/latitude.all.best"));

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Latitude");


const QString& Latitude::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Latitude >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}


Object * Latitude::JSONToObject(const QByteArray & jsonData)
{
    QJson::Parser parser;
    QVariantMap data;
    bool ok;

    data = parser.parse(jsonData, &ok).toMap();

    if (!ok) {
        return 0;
    }

    const QVariantMap info = data.value(QLatin1String("data")).toMap();

    return parseLocation(info);
}

QByteArray Latitude::objectToJSON(Object * object)
{
    QVariantMap map, output;
    Objects::Location *location = static_cast<Objects::Location*>(object);

    map.insert(QLatin1String("kind"), QLatin1String("latitude#location"));
    map.insert(QLatin1String("latitude"), QString::number(location->latitude()));
    map.insert(QLatin1String("longitude"), QString::number(location->longitude()));

    if (location->timestamp() != 0) {
        map.insert(QLatin1String("timestampMs"), location->timestamp());
    }
    if (location->accuracy() != -1) {
        map.insert(QLatin1String("accuracy"), location->accuracy());
    }
    if (location->speed() != -1) {
        map.insert(QLatin1String("speed"), location->speed());
    }
    if (location->heading() != -1) {
        map.insert(QLatin1String("heading"), location->heading());
    }

    map.insert(QLatin1String("altitude"), location->altitude());

    if (location->altitudeAccuracy() != 0) {
        map.insert(QLatin1String("altitudeAccuracy"), location->altitudeAccuracy());
    }

    output.insert(QLatin1String("data"), map);

    QJson::Serializer serializer;
    return serializer.serialize(output);
}

QList< Object * > Latitude::parseJSONFeed(const QByteArray & jsonFeed, FeedData & feedData)
{
    Q_UNUSED(feedData);

    QList< KGAPI::Object* > output;
    QJson::Parser parser;

    const QVariantMap map = parser.parse(jsonFeed).toMap();
    const QVariantMap data = map.value(QLatin1String("data")).toMap();
    const QVariantList items = data.value(QLatin1String("items")).toList();
    Q_FOREACH(const QVariant &c, items) {
        QVariantMap location = c.toMap();
        output << parseLocation(location);
    }

    return output;
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

    if (map.contains(QLatin1String("timestampMs"))) {
        object->setTimestamp(map.value(QLatin1String("timestampMs")).toULongLong());
    }
    if (map.contains(QLatin1String("latitude"))) {
        object->setLatitude(map.value(QLatin1String("latitude")).toFloat());
    }
    if (map.contains(QLatin1String("longitude"))) {
        object->setLongitude(map.value(QLatin1String("longitude")).toFloat());
    }
    if (map.contains(QLatin1String("accuracy"))) {
        object->setAccuracy(map.value(QLatin1String("accuracy")).toInt());
    }
    if (map.contains(QLatin1String("speed"))) {
        object->setSpeed(map.value(QLatin1String("speed")).toInt());
    }
    if (map.contains(QLatin1String("heading"))) {
        object->setHeading(map.value(QLatin1String("heading")).toInt());
    }
    if (map.contains(QLatin1String("altitude"))) {
        object->setAltitude(map.value(QLatin1String("altitude")).toInt());
    }
    if (map.contains(QLatin1String("altitudeAccuracy"))) {
        object->setAltitudeAccuracy(map.value(QLatin1String("altitudeAccuracy")).toInt());
    }

    return object;
}


QString Latitude::protocolVersion() const
{
    return QLatin1String("1");
}

const QUrl & Latitude::scopeUrl() const
{
    return Latitude::ScopeUrl;
}

QUrl Latitude::retrieveCurrentLocationUrl(const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/currentLocation");

    if (granularity == City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
    } else if (granularity == Best) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("best"));
    }

    return QUrl(url);
}

QUrl Latitude::deleteCurrentLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl Latitude::insertCurrentLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl Latitude::locationHistoryUrl(const Latitude::Granularity granularity, const int maxResults,
                                  const qlonglong maxTime, const qlonglong minTime)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location");

    if (granularity == City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
    } else if (granularity == Best) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("best"));
    }

    if (maxResults > 0) {
        url.addQueryItem(QLatin1String("max-results"), QString::number(maxResults));
    }

    if ((maxTime > 0) && (maxTime >= minTime)) {
        url.addQueryItem(QLatin1String("max-time"), QString::number(maxTime));
    }

    if ((minTime > 0) && (minTime <= maxTime)) {
        url.addQueryItem(QLatin1String("min-time"), QString::number(minTime));
    }

    return url;
}

QUrl Latitude::retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));

     if (granularity == City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
     } else if (granularity == Best) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("best"));
     }

    return url;
}

QUrl Latitude::insertLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/location");
}

QUrl Latitude::deleteLocationUrl(const qlonglong id)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));

    return url;
}
