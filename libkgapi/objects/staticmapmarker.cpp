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


#include "staticmapmarker.h"
#include "staticmapmarker_p.h"

using namespace KGAPI::Objects;

StaticMapMarkerPrivate::StaticMapMarkerPrivate():
    locationType(StaticMapMarker::Undefined),
    size(StaticMapMarker::Normal),
    color(Qt::red)
{ }

StaticMapMarkerPrivate::StaticMapMarkerPrivate(const StaticMapMarkerPrivate & other):
    locationType(other.locationType),
    size(other.size),
    color(other.color),
    label(other.label),
    locationsString(other.locationsString),
    locationsAddress(other.locationsAddress),
    locationsGeo(other.locationsGeo)
{ }

StaticMapMarkerPrivate::~StaticMapMarkerPrivate()
{ }

StaticMapMarker::StaticMapMarker():
    d(new StaticMapMarkerPrivate)
{ }

StaticMapMarker::StaticMapMarker (const QString& address, const QChar& label, const MarkerSize size, const QColor& color)
{
    StaticMapMarker(QStringList(address), label, size, color);
}

StaticMapMarker::StaticMapMarker (const KABC::Address& address, const QChar& label, const MarkerSize size, const QColor& color)
{
    QList<KABC::Address> list;
    list << address;
    StaticMapMarker(list, label, size, color);
}

StaticMapMarker::StaticMapMarker (const KABC::Geo& address, const QChar& label, const MarkerSize size, const QColor& color)
{
    QList<KABC::Geo> list;
    list << address;
    StaticMapMarker(list, label, size, color);
}

StaticMapMarker::StaticMapMarker(const QStringList & locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new StaticMapMarkerPrivate)
{
    d->locationType = String;
    d->locationsString = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QList< KABC::Address >& locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new StaticMapMarkerPrivate)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QList< KABC::Geo >& locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new StaticMapMarkerPrivate)
{
    d->locationType = KABCGeo;
    d->locationsGeo = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const StaticMapMarker& other):
    d(new StaticMapMarkerPrivate(*(other.d)))
{ }

StaticMapMarker::~StaticMapMarker()
{
    delete d;
}

StaticMapMarker::LocationType StaticMapMarker::locationType() const
{
    return d->locationType;
}

QColor StaticMapMarker::color() const
{
    return d->color;
}

void StaticMapMarker::setColor(const QColor& color)
{
    d->color = color;
}

bool StaticMapMarker::isValid() const
{
    return (d->locationType != Undefined);
}

QChar StaticMapMarker::label() const
{
    return d->label;
}

void StaticMapMarker::setLabel(const QChar& label)
{
    d->label = label;
}

QStringList StaticMapMarker::locationsString() const
{
    return d->locationsString;
}

void StaticMapMarker::setLocation(const QString& location)
{
    d->locationType = String;
    d->locationsString.clear();
    d->locationsString << location;
    d->locationsAddress.clear();
    d->locationsGeo.clear();
}

void StaticMapMarker::setLocations(const QStringList& locations)
{
    d->locationType = KABCAddress;
    d->locationsString = locations;
    d->locationsAddress.clear();
    d->locationsGeo.clear();
}

QList< KABC::Address > StaticMapMarker::locationsAddress() const
{
    return d->locationsAddress;
}

void StaticMapMarker::setLocation(const KABC::Address& location)
{
    d->locationType = KABCAddress;
    d->locationsAddress.clear();
    d->locationsAddress << location;
    d->locationsString.clear();
    d->locationsGeo.clear();
}

void StaticMapMarker::setLocations(const QList< KABC::Address >& locations)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->locationsString.clear();
    d->locationsGeo.clear();
}

QList< KABC::Geo > StaticMapMarker::locationsGeo() const
{
    return d->locationsGeo;
}

void StaticMapMarker::setLocation(const KABC::Geo& location)
{
    d->locationType = KABCGeo;
    d->locationsGeo.clear();
    d->locationsGeo << location;
    d->locationsString.clear();
    d->locationsAddress.clear();
}

void StaticMapMarker::setLocations(const QList< KABC::Geo >& locations)
{
    d->locationType = KABCGeo;
    d->locationsGeo = locations;
    d->locationsString.clear();
    d->locationsAddress.clear();
}

StaticMapMarker::MarkerSize StaticMapMarker::size() const
{
    return d->size;
}

void StaticMapMarker::setSize(const MarkerSize size)
{
    d->size = size;
}

QString StaticMapMarker::toString() const
{
    QString ret;

    switch (d->size) {
    case Tiny:
        ret += "size:tiny|";
        break;
    case Small:
        ret += "size:small|";
        break;
    case Middle:
        ret += "size:mid|";
        break;
    case Normal:
        break;
    }

    if (d->color != Qt::red)
        ret += "color:" + d->color.name().replace("#", "0x") + "|";

    if (d->label.isLetterOrNumber() && d->size > 1)
        ret += "label:" + QString(d->label.toUpper()) + "|";

    if (d->locationType == String) {

        Q_FOREACH(const QString & addr, d->locationsString) {
            ret += addr + "|";
        }

    } else if (d->locationType == KABCAddress) {

        Q_FOREACH(const KABC::Address & addr, d->locationsAddress) {
            ret += addr.formattedAddress() + "|";
        }

    } else if (d->locationType == KABCGeo) {

        Q_FOREACH(const KABC::Geo & addr, d->locationsGeo) {
            ret += QString::number(addr.latitude()) + "," +
                  QString::number(addr.longitude()) + "|";
        }

    }

    ret = ret.replace(", ", ",");
    ret = ret.replace(". ", ".");
    ret = ret.replace(" ", "+");
    ret = ret.replace("\n",",");
    ret = ret.remove(ret.lastIndexOf("|"), 1);

    return ret;

}