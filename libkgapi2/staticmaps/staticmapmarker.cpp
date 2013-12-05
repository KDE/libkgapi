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

using namespace KGAPI2;

class StaticMapMarker::Private
{
public:
    Private();
    Private(const Private &other);

    void init(const Private &other);

    LocationType locationType;
    MarkerSize size;

    QColor color;
    QChar label;

    QStringList locationsString;
    QList<KABC::Address> locationsAddress;
    QList<KABC::Geo> locationsGeo;
};

StaticMapMarker::Private::Private():
    locationType(StaticMapMarker::Undefined),
    size(StaticMapMarker::Normal),
    color(Qt::red)
{ }

StaticMapMarker::Private::Private(const Private & other)
{
    init(other);
}

void StaticMapMarker::Private::init(const Private &other)
{
    locationType = other.locationType;
    size = other.size;
    color = other.color;
    label = other.label;
    locationsString = other.locationsString;
    locationsAddress = other.locationsAddress;
    locationsGeo = other.locationsGeo;
}

StaticMapMarker::StaticMapMarker():
    d(new Private)
{
}

StaticMapMarker::StaticMapMarker (const QString& address, const QChar& label, const MarkerSize size, const QColor& color):
    d(new Private)
{
    QStringList list;
    list << address;
    d->locationType = String;
    d->locationsString = list;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker (const KABC::Address& address, const QChar& label, const MarkerSize size, const QColor& color):
    d(new Private)
{
    QList<KABC::Address> list;
    list << address;
    d->locationType = KABCAddress;
    d->locationsAddress = list;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker (const KABC::Geo& address, const QChar& label, const MarkerSize size, const QColor& color):
    d(new Private)
{
    QList<KABC::Geo> list;
    list << address;
    d->locationType = KABCGeo;
    d->locationsGeo = list;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QStringList & locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new Private)
{
    d->locationType = String;
    d->locationsString = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QList< KABC::Address >& locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new Private)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QList< KABC::Geo >& locations, const QChar& label,
                                 const MarkerSize size, const QColor& color):
    d(new Private)
{
    d->locationType = KABCGeo;
    d->locationsGeo = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const StaticMapMarker& other):
    d(new Private(*(other.d)))
{
}

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
        ret += QLatin1String("size:tiny|");
        break;
    case Small:
        ret += QLatin1String("size:small|");
        break;
    case Middle:
        ret += QLatin1String("size:mid|");
        break;
    case Normal:
        break;
    }

    if (d->color != Qt::red)
        ret += QLatin1String("color:") + d->color.name().replace(QLatin1Char('#'), QLatin1String("0x")) + QLatin1String("|");

    if (d->label.isLetterOrNumber() && d->size > 1)
        ret += QLatin1String("label:") + d->label.toUpper() + QLatin1String("|");

    if (d->locationType == String) {

        Q_FOREACH(const QString & addr, d->locationsString) {
            ret += addr + QLatin1String("|");
        }

    } else if (d->locationType == KABCAddress) {

        Q_FOREACH(const KABC::Address & addr, d->locationsAddress) {
            ret += addr.formattedAddress() + QLatin1String("|");
        }

    } else if (d->locationType == KABCGeo) {

        Q_FOREACH(const KABC::Geo & addr, d->locationsGeo) {
            ret += QString::number(addr.latitude()) + QLatin1String(",") +
                  QString::number(addr.longitude()) + QLatin1String("|");
        }

    }

    ret = ret.replace(QLatin1String(", "), QLatin1String(","));
    ret = ret.replace(QLatin1String(". "), QLatin1String("."));
    ret = ret.replace(QLatin1Char(' '), QLatin1Char('+'));
    ret = ret.replace(QLatin1Char('\n'), QLatin1Char(','));
    ret = ret.remove(ret.lastIndexOf(QLatin1Char('|')), 1);

    return ret;

}

StaticMapMarker& StaticMapMarker::operator=(const StaticMapMarker& other)
{
    if (&other == this) {
        return *this;
    }

    d->init(*(other.d));
    return *this;
}
