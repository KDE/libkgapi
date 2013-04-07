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

#include "staticmapurl.h"

#include <KDE/KUrl>

using namespace KGAPI::Objects;

#include "common/staticmapurl.inc.cpp"

StaticMapUrl::StaticMapUrl(const QString& location, const QSize size,
                           const quint32 zoom, const bool sensor):
    d(new Private)
{
    setLocation(location);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KABC::Address& address, const QSize size,
                           const quint32 zoom, const bool sensor):
    d(new Private)
{
    setLocation(address);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KABC::Geo& geo, const QSize size,
                           const quint32 zoom, const bool sensor):
    d(new Private)
{
    setLocation(geo);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}
