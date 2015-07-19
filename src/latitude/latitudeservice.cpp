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


#include <QJsonDocument>

namespace KGAPI2 {

namespace LatitudeService {

namespace Private
{
    LocationPtr parseLocation(const QVariantMap &map);

    static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));
    static const QString LocationBasePath(QStringLiteral("/latitude/v1/location"));
    static const QString CurrentLocationBasePath(QStringLiteral("/latitude/v1/currentLocation"));
}

LocationPtr JSONToLocation(const QByteArray & jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (document.isNull()) {
        return LocationPtr();
    }
    const QVariantMap data = document.toVariant().toMap();
    const QVariantMap info = data.value(QStringLiteral("data")).toMap();

    return Private::parseLocation(info);
}

QByteArray locationToJSON(const LocationPtr &location)
{
    QVariantMap map, output;

    map.insert(QStringLiteral("kind"), QStringLiteral("latitude#location"));
    map.insert(QStringLiteral("latitude"), QString::number(location->latitude()));
    map.insert(QStringLiteral("longitude"), QString::number(location->longitude()));

    if (location->timestamp() != 0) {
        map.insert(QStringLiteral("timestampMs"), location->timestamp());
    }
    if (location->accuracy() != -1) {
        map.insert(QStringLiteral("accuracy"), location->accuracy());
    }
    if (location->speed() != -1) {
        map.insert(QStringLiteral("speed"), location->speed());
    }
    if (location->heading() != -1) {
        map.insert(QStringLiteral("heading"), location->heading());
    }

    map.insert(QStringLiteral("altitude"), location->altitude());

    if (location->altitudeAccuracy() != 0) {
        map.insert(QStringLiteral("altitudeAccuracy"), location->altitudeAccuracy());
    }

    output.insert(QStringLiteral("data"), map);

    QJsonDocument document = QJsonDocument::fromVariant(output);
    return document.toJson(QJsonDocument::Compact);
}

ObjectsList parseLocationJSONFeed(const QByteArray & jsonFeed, FeedData & feedData)
{
    Q_UNUSED(feedData);

    ObjectsList output;

    QJsonDocument document = QJsonDocument::fromJson(jsonFeed);
    const QVariantMap map = document.toVariant().toMap();
    const QVariantMap data = map.value(QStringLiteral("data")).toMap();
    const QVariantList items = data.value(QStringLiteral("items")).toList();
    Q_FOREACH(const QVariant &c, items) {
        QVariantMap location = c.toMap();
        output << Private::parseLocation(location).dynamicCast<Object>();
    }

    return output;
}

LocationPtr Private::parseLocation(const QVariantMap &map)
{
    LocationPtr location(new Location);

    if (map.contains(QStringLiteral("timestampMs"))) {
        location->setTimestamp(map.value(QStringLiteral("timestampMs")).toULongLong());
    }
    if (map.contains(QStringLiteral("latitude"))) {
        location->setLatitude(map.value(QStringLiteral("latitude")).toFloat());
    }
    if (map.contains(QStringLiteral("longitude"))) {
        location->setLongitude(map.value(QStringLiteral("longitude")).toFloat());
    }
    if (map.contains(QStringLiteral("accuracy"))) {
        location->setAccuracy(map.value(QStringLiteral("accuracy")).toInt());
    }
    if (map.contains(QStringLiteral("speed"))) {
        location->setSpeed(map.value(QStringLiteral("speed")).toInt());
    }
    if (map.contains(QStringLiteral("heading"))) {
        location->setHeading(map.value(QStringLiteral("heading")).toInt());
    }
    if (map.contains(QStringLiteral("altitude"))) {
        location->setAltitude(map.value(QStringLiteral("altitude")).toInt());
    }
    if (map.contains(QStringLiteral("altitudeAccuracy"))) {
        location->setAltitudeAccuracy(map.value(QStringLiteral("altitudeAccuracy")).toInt());
    }

    return location;
}


QString APIVersion()
{
    return QStringLiteral("1");
}

QUrl retrieveCurrentLocationUrl(const Latitude::Granularity granularity)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CurrentLocationBasePath);
    if (granularity == Latitude::City) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
    } else if (granularity == Latitude::Best) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
    }

    return url;
}

QUrl deleteCurrentLocationUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CurrentLocationBasePath);
    return url;
}

QUrl insertCurrentLocationUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::CurrentLocationBasePath);
    return url;
}

QUrl locationHistoryUrl(const Latitude::Granularity granularity, const int maxResults,
                        const qlonglong maxTime, const qlonglong minTime)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath);

    if (granularity == Latitude::City) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
    } else if (granularity == Latitude::Best) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
    }

    if (maxResults > 0) {
        url.addQueryItem(QStringLiteral("max-results"), QString::number(maxResults));
    }

    if ((maxTime > 0) && (maxTime >= minTime)) {
        url.addQueryItem(QStringLiteral("max-time"), QString::number(maxTime));
    }

    if ((minTime > 0) && (minTime <= maxTime)) {
        url.addQueryItem(QStringLiteral("min-time"), QString::number(minTime));
    }

    return url;
}

QUrl retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath % QLatin1Char('/') % QString::number(id));

     if (granularity == Latitude::City) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
     } else if (granularity == Latitude::Best) {
        url.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
     }

    return url;
}

QUrl insertLocationUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath);
    return url;
}

QUrl deleteLocationUrl(const qlonglong id)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath);
    return url;
}

} // namespace LatitudeService

} // namespace KGAPI2
