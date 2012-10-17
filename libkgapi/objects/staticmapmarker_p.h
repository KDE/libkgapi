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


#ifndef STATICMAPMARKER_P_H
#define STATICMAPMARKER_P_H

#include <QColor>

#include <KABC/Address>
#include <KABC/Geo>

namespace KGAPI
{

namespace Objects
{

class StaticMapMarkerPrivate
{
public:
    StaticMapMarkerPrivate();
    StaticMapMarkerPrivate(const StaticMapMarkerPrivate &other);
    ~StaticMapMarkerPrivate();

    KGAPI::Objects::StaticMapMarker::LocationType locationType;

    KGAPI::Objects::StaticMapMarker::MarkerSize size;

    QColor color;

    QChar label;

    QStringList locationsString;

    QList<KABC::Address> locationsAddress;

    QList<KABC::Geo> locationsGeo;
};

} /* namespace Objects */

} /* namespace KGAPI */

#endif // STATICMAPMARKER_P_H
