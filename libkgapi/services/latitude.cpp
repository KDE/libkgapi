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

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <libkgapi2/latitude/latitudeservice.h>
#include <libkgapi2/latitude/location.h>
#include <libkgapi2/account.h>

using namespace KGAPI;
using namespace Services;

QUrl Latitude::ScopeUrl(KGAPI2::Account::latitudeScopeUrl());

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Latitude");


QString Latitude::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Latitude >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}


Object * Latitude::JSONToObject(const QByteArray & jsonData)
{
    KGAPI2::LocationPtr location = KGAPI2::LatitudeService::JSONToLocation(jsonData);
    return new Objects::Location(*reinterpret_cast<Objects::Location*>(location.data()));
}

QByteArray Latitude::objectToJSON(Object * object)
{
    KGAPI2::LocationPtr location(new KGAPI2::Location(*reinterpret_cast<KGAPI2::Location*>(object)));
    return KGAPI2::LatitudeService::locationToJSON(location);
}

QList< Object * > Latitude::parseJSONFeed(const QByteArray & jsonFeed, FeedData & feedData)
{
    QList< KGAPI::Object* > list;
    KGAPI2::ObjectsList objects = KGAPI2::LatitudeService::parseLocationJSONFeed(jsonFeed, feedData);

    Q_FOREACH(const KGAPI2::ObjectPtr &object, objects) {
        KGAPI2::LocationPtr location = object.dynamicCast<KGAPI2::Location>();
        list << new Objects::Location(*reinterpret_cast<Objects::Location*>(location.data()));
    }

    return list;
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
    return KGAPI2::LatitudeService::APIVersion();
}

QUrl Latitude::scopeUrl() const
{
    return Latitude::ScopeUrl;
}

QUrl Latitude::retrieveCurrentLocationUrl(const Latitude::Granularity granularity)
{
    return KGAPI2::LatitudeService::retrieveCurrentLocationUrl(static_cast<KGAPI2::Latitude::Granularity>(granularity));
}

QUrl Latitude::deleteCurrentLocationUrl()
{
    return KGAPI2::LatitudeService::deleteCurrentLocationUrl();
}

QUrl Latitude::insertCurrentLocationUrl()
{
    return KGAPI2::LatitudeService::insertCurrentLocationUrl();
}

QUrl Latitude::locationHistoryUrl(const Latitude::Granularity granularity, const int maxResults,
                                  const qlonglong maxTime, const qlonglong minTime)
{
    return KGAPI2::LatitudeService::locationHistoryUrl(
            static_cast<KGAPI2::Latitude::Granularity>(granularity), maxResults,
            maxTime, minTime);
}

QUrl Latitude::retrieveLocationUrl(const qlonglong id, const Latitude::Granularity granularity)
{
    return KGAPI2::LatitudeService::retrieveLocationUrl(id, static_cast<KGAPI2::Latitude::Granularity>(granularity));
}

QUrl Latitude::insertLocationUrl()
{
    return KGAPI2::LatitudeService::insertLocationUrl();
}

QUrl Latitude::deleteLocationUrl(const qlonglong id)
{
    return KGAPI2::LatitudeService::deleteLocationUrl(id);
}
