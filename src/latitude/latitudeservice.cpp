/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "latitudeservice.h"
#include "location.h"


#include <QJsonDocument>
#include <QUrlQuery>

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
    output.reserve(items.size());
    for (const QVariant &c : items) {
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
    QUrlQuery query(url);
    if (granularity == Latitude::City) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
    } else if (granularity == Latitude::Best) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
    }
    url.setQuery(query);

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
    QUrlQuery query(url);
    if (granularity == Latitude::City) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
    } else if (granularity == Latitude::Best) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
    }

    if (maxResults > 0) {
        query.addQueryItem(QStringLiteral("max-results"), QString::number(maxResults));
    }

    if ((maxTime > 0) && (maxTime >= minTime)) {
        query.addQueryItem(QStringLiteral("max-time"), QString::number(maxTime));
    }

    if ((minTime > 0) && (minTime <= maxTime)) {
        query.addQueryItem(QStringLiteral("min-time"), QString::number(minTime));
    }
    url.setQuery(query);

    return url;
}

QUrl retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath % QLatin1Char('/') % QString::number(id));
    QUrlQuery query(url);
     if (granularity == Latitude::City) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("city"));
     } else if (granularity == Latitude::Best) {
        query.addQueryItem(QStringLiteral("granularity"), QStringLiteral("best"));
     }
     url.setQuery(query);

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
    Q_UNUSED(id);
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::LocationBasePath);
    return url;
}

} // namespace LatitudeService

} // namespace KGAPI2
