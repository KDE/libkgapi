/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "staticmapmarker.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN StaticMapMarker::Private
{
public:
    Private();
    Private(const Private &other);

    void init(const Private &other);

    LocationType locationType = StaticMapMarker::Undefined;
    MarkerSize size = StaticMapMarker::Normal;

    QColor color = Qt::red;
    QChar label;

    QStringList locationsString;
    KContacts::Address::List locationsAddress;
    QList<KContacts::Geo> locationsGeo;
};

StaticMapMarker::Private::Private()
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

StaticMapMarker::StaticMapMarker (const KContacts::Address& address, QChar label, const MarkerSize size, const QColor& color):
    d(new Private)
{
    KContacts::Address::List list;
    list << address;
    d->locationType = KABCAddress;
    d->locationsAddress = list;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker (const KContacts::Geo& address, QChar label, const MarkerSize size, const QColor& color):
    d(new Private)
{
    QList<KContacts::Geo> list;
    list << address;
    d->locationType = KABCGeo;
    d->locationsGeo = list;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QStringList & locations, QChar label,
                                 const MarkerSize size, const QColor& color):
    d(new Private)
{
    d->locationType = String;
    d->locationsString = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const KContacts::Address::List & locations, QChar label,
                                 const MarkerSize size, const QColor& color):
    d(new Private)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->label = label;
    d->size = size;
    d->color = color;
}

StaticMapMarker::StaticMapMarker(const QList< KContacts::Geo >& locations, QChar label,
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

void StaticMapMarker::setLabel(QChar label)
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

KContacts::Address::List StaticMapMarker::locationsAddress() const
{
    return d->locationsAddress;
}

void StaticMapMarker::setLocation(const KContacts::Address& location)
{
    d->locationType = KABCAddress;
    d->locationsAddress.clear();
    d->locationsAddress << location;
    d->locationsString.clear();
    d->locationsGeo.clear();
}

void StaticMapMarker::setLocations(const KContacts::Address::List &locations)
{
    d->locationType = KABCAddress;
    d->locationsAddress = locations;
    d->locationsString.clear();
    d->locationsGeo.clear();
}

QList< KContacts::Geo > StaticMapMarker::locationsGeo() const
{
    return d->locationsGeo;
}

void StaticMapMarker::setLocation(const KContacts::Geo& location)
{
    d->locationType = KABCGeo;
    d->locationsGeo.clear();
    d->locationsGeo << location;
    d->locationsString.clear();
    d->locationsAddress.clear();
}

void StaticMapMarker::setLocations(const QList< KContacts::Geo >& locations)
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
        ret += QLatin1String("color:") + d->color.name().replace(QLatin1Char('#'), QLatin1String("0x")) + QLatin1Char('|');

    if (d->label.isLetterOrNumber() && d->size > 1)
        ret += QLatin1String("label:") + d->label.toUpper() + QLatin1Char('|');

    if (d->locationType == String) {

        for (const QString & addr : std::as_const(d->locationsString)) {
            ret += addr + QLatin1Char('|');
        }

    } else if (d->locationType == KABCAddress) {

        for (const KContacts::Address & addr : std::as_const(d->locationsAddress)) {
            ret += addr.formattedAddress() + QLatin1Char('|');
        }

    } else if (d->locationType == KABCGeo) {

        for (const KContacts::Geo & addr : std::as_const(d->locationsGeo)) {
            ret += QString::number(addr.latitude()) + QLatin1Char(',') +
                  QString::number(addr.longitude()) + QLatin1Char('|');
        }

    }

    ret.replace(QLatin1String(", "), QLatin1String(","));
    ret.replace(QLatin1String(". "), QLatin1String("."));
    ret.replace(QLatin1Char(' '), QLatin1Char('+'));
    ret.replace(QLatin1Char('\n'), QLatin1Char(','));
    ret.remove(ret.lastIndexOf(QLatin1Char('|')), 1);

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
