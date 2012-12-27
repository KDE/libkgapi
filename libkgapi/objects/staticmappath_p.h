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


#ifndef STATICMAPPATH_P_H
#define STATICMAPPATH_P_H

#include <QColor>
#include <QSharedData>

#include <KABC/Address>
#include <KABC/Geo>

namespace KGAPI
{

namespace Objects
{

class StaticMapPathPrivate
{
public:
    StaticMapPathPrivate();
    StaticMapPathPrivate(const StaticMapPathPrivate &other);
    ~StaticMapPathPrivate();

    void init(const StaticMapPathPrivate &other);

    KGAPI::Objects::StaticMapPath::LocationType locationType;

    QColor color;
    QColor fillColor;
    quint8 weight;

    QStringList locationsString;
    QList<KABC::Address> locationsAddress;
    QList<KABC::Geo> locationsGeo;
};

} /* namespace Objects */

} /* namespace KGAPI */

#endif // STATICMAPPATH_P_H
