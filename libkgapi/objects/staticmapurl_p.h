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

#ifndef LIBKGAPI_OBJECTS_STATICMAPURL_P_H
#define LIBKGAPI_OBJECTS_STATICMAPURL_P_H

#include <QSize>
#include <QSharedData>
#include <QUrl>
#include <QColor>
#include <QStringList>

#include <KABC/Address>
#include <KABC/Geo>

#include "staticmapmarker.h"
#include "staticmapmarker_p.h"
#include "staticmappath.h"
#include "staticmappath_p.h"

namespace KGAPI
{

namespace Objects
{

class StaticMapUrlPrivate
{
public:
    StaticMapUrlPrivate();
    StaticMapUrlPrivate(const StaticMapUrlPrivate& other);
    ~StaticMapUrlPrivate();

    KGAPI::Objects::StaticMapUrl::LocationType locationType;

    KGAPI::Objects::StaticMapUrl::ImageFormat format;

    QString locationString;

    KABC::Address locationAddress;

    KABC::Geo locationGeo;

    KGAPI::Objects::StaticMapUrl::MapType maptype;

    QList<StaticMapMarker> markers;

    QList<StaticMapPath> paths;

    KGAPI::Objects::StaticMapUrl::Scale scale;

    bool sensor;

    QSize size;

    QString visibleString;

    KABC::Address visibleAddress;

    KABC::Geo visibleGeo;

    KGAPI::Objects::StaticMapUrl::LocationType visibleLocationType;

    qint32 zoom;
};

} /* namespace Objects */

} /* namespace KGAPI */

#endif // LIBKGAPI_OBJECTS_STATICMAPURL_P_H

