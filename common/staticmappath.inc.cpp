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

class StaticMapPath::Private
{
  public:
    Private();
    Private(const Private &other);

    void init(const Private &other);

    StaticMapPath::LocationType locationType;

    QColor color;
    QColor fillColor;
    quint8 weight;

    QStringList locationsString;
    QList<KABC::Address> locationsAddress;
    QList<KABC::Geo> locationsGeo;
};


StaticMapPath::Private::Private():
    color(Qt::blue),
    weight(5)
{
}

StaticMapPath::Private::Private(const Private & other)
{
    init(other);
}

void StaticMapPath::Private::init(const Private &other)
{
    locationType = other.locationType;
    color = other.color;
    fillColor = other.fillColor;
    weight = other.weight;
    locationsString = other.locationsString;
    locationsAddress = other.locationsAddress;
    locationsGeo = other.locationsGeo;
}

StaticMapPath::StaticMapPath():
    d(new Private)
{
}

StaticMapPath::StaticMapPath(const StaticMapPath& other):
    d(new Private(*(other.d)))
{
}

StaticMapPath::StaticMapPath(const QStringList & locations, const quint8 weight,
                             const QColor & color, const QColor & fillColor):
    d(new Private)
{
    d->locationType = String;
    d->locationsString = locations;
    d->weight = weight;
    d->color = color;
    d->fillColor = fillColor;
}

StaticMapPath::StaticMapPath(const QList< KABC::Address >& locations, const quint8 weight,
                             const QColor & color, const QColor & fillColor):
    d(new Private)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->weight = weight;
    d->color = color;
    d->fillColor = fillColor;
}

StaticMapPath::StaticMapPath(const QList< KABC::Geo >& locations, const quint8 weight,
                             const QColor & color, const QColor & fillColor):
    d(new Private)
{
    d->locationType = KABCGeo;
    d->locationsGeo = locations;
    d->weight = weight;
    d->color = color;
    d->fillColor = fillColor;
}

StaticMapPath::~StaticMapPath()
{
    delete d;
}

StaticMapPath::LocationType StaticMapPath::locationType() const
{
    return d->locationType;
}

QColor StaticMapPath::color() const
{
    return d->color;
}

void StaticMapPath::setColor(const QColor & color)
{
    d->color = color;
}

bool StaticMapPath::isValid() const
{
    return (d->locationType != Undefined);
}

QColor StaticMapPath::fillColor() const
{
    return d->fillColor;
}

void StaticMapPath::setFillColor(const QColor & color)
{
    d->fillColor = color;
}

QStringList StaticMapPath::locationsString() const
{
    return d->locationsString;
}

void StaticMapPath::setLocations(const QStringList & locations)
{
    d->locationType = String;
    d->locationsString = locations;
    d->locationsAddress.clear();
    d->locationsGeo.clear();
}

QList< KABC::Address > StaticMapPath::locationsAddress() const
{
    return d->locationsAddress;
}

void StaticMapPath::setLocations(const QList< KABC::Address >& locations)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->locationsString.clear();
    d->locationsGeo.clear();
}

QList< KABC::Geo > StaticMapPath::locationsGeo() const
{
    return d->locationsGeo;
}

void StaticMapPath::setLocations(const QList< KABC::Geo >& locations)
{
    d->locationType = KABCGeo;
    d->locationsGeo = locations;
    d->locationsString.clear();
    d->locationsAddress.clear();
}

QString StaticMapPath::toString() const
{
    QString ret;

    if (d->color != Qt::blue)
        ret += QLatin1String("color:") + d->color.name().replace(QLatin1Char('#'), QLatin1String("0x")) + QLatin1String("|");
    if (d->weight != 5)
        ret += QLatin1String("weight:") + QString::number(d->weight) + QLatin1String("|");
    if (d->fillColor.isValid())
        ret += QLatin1String("fillcolor:") + d->fillColor.name().replace(QLatin1Char('#'), QLatin1String("0x")) + QLatin1String("|");

    if (locationType() == String) {

        Q_FOREACH(const QString & addr, d->locationsString) {
            ret += addr + QLatin1String("|");
        }

    } else if (locationType() == KABCAddress) {

        Q_FOREACH(const KABC::Address & addr, d->locationsAddress) {
            ret += addr.formattedAddress() + QLatin1String("|");
        }

    } else if (locationType() == KABCGeo) {

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

quint8 StaticMapPath::weight() const
{
    return d->weight;
}

void StaticMapPath::setWeight(const quint8 weight)
{
    d->weight = weight;
}

StaticMapPath& StaticMapPath::operator=(const StaticMapPath& other)
{
    if (&other == this) {
        return *this;
    }

    d->init(*(other.d));
    return *this;
}
