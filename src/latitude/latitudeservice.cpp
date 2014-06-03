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


#include "latitudeservice.h"
#include "location.h"

#include <KDE/KDateTime>
#include <KDE/KUrl>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace KGAPI2 {

namespace LatitudeService {

namespace Private
{
    LocationPtr parseLocation(const QVariantMap map);
}

LocationPtr JSONToLocation(const QByteArray & jsonData)
{
    QJson::Parser parser;
    QVariantMap data;
    bool ok;

    data = parser.parse(jsonData, &ok).toMap();

    if (!ok) {
        return LocationPtr();
    }

    const QVariantMap info = data.value(QLatin1String("data")).toMap();

    return Private::parseLocation(info);
}

QByteArray locationToJSON(const LocationPtr &location)
{
    QVariantMap map, output;

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

ObjectsList parseLocationJSONFeed(const QByteArray & jsonFeed, FeedData & feedData)
{
    Q_UNUSED(feedData);

    ObjectsList output;
    QJson::Parser parser;

    const QVariantMap map = parser.parse(jsonFeed).toMap();
    const QVariantMap data = map.value(QLatin1String("data")).toMap();
    const QVariantList items = data.value(QLatin1String("items")).toList();
    Q_FOREACH(const QVariant &c, items) {
        QVariantMap location = c.toMap();
        output << Private::parseLocation(location).dynamicCast<Object>();
    }

    return output;
}

LocationPtr Private::parseLocation(const QVariantMap map)
{
    LocationPtr location(new Location);

    if (map.contains(QLatin1String("timestampMs"))) {
        location->setTimestamp(map.value(QLatin1String("timestampMs")).toULongLong());
    }
    if (map.contains(QLatin1String("latitude"))) {
        location->setLatitude(map.value(QLatin1String("latitude")).toFloat());
    }
    if (map.contains(QLatin1String("longitude"))) {
        location->setLongitude(map.value(QLatin1String("longitude")).toFloat());
    }
    if (map.contains(QLatin1String("accuracy"))) {
        location->setAccuracy(map.value(QLatin1String("accuracy")).toInt());
    }
    if (map.contains(QLatin1String("speed"))) {
        location->setSpeed(map.value(QLatin1String("speed")).toInt());
    }
    if (map.contains(QLatin1String("heading"))) {
        location->setHeading(map.value(QLatin1String("heading")).toInt());
    }
    if (map.contains(QLatin1String("altitude"))) {
        location->setAltitude(map.value(QLatin1String("altitude")).toInt());
    }
    if (map.contains(QLatin1String("altitudeAccuracy"))) {
        location->setAltitudeAccuracy(map.value(QLatin1String("altitudeAccuracy")).toInt());
    }

    return location;
}


QString APIVersion()
{
    return QLatin1String("1");
}

QUrl retrieveCurrentLocationUrl(const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/currentLocation");

    if (granularity == Latitude::City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
    } else if (granularity == Latitude::Best) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("best"));
    }

    return QUrl(url);
}

QUrl deleteCurrentLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl insertCurrentLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/currentLocation");
}

QUrl locationHistoryUrl(const Latitude::Granularity granularity, const int maxResults,
                        const qlonglong maxTime, const qlonglong minTime)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location");

    if (granularity == Latitude::City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
    } else if (granularity == Latitude::Best) {
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

QUrl retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));

     if (granularity == Latitude::City) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("city"));
     } else if (granularity == Latitude::Best) {
        url.addQueryItem(QLatin1String("granularity"), QLatin1String("best"));
     }

    return url;
}

QUrl insertLocationUrl()
{
    return KUrl("https://www.googleapis.com/latitude/v1/location");
}

QUrl deleteLocationUrl(const qlonglong id)
{
    KUrl url("https://www.googleapis.com/latitude/v1/location/");
    url.addPath(QString::number(id));

    return url;
}

} // namespace LatitudeService

} // namespace KGAPI2
