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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "location.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN Location::Private
{
  public:
    Private();
    Private(const Private &other);

    qulonglong timestamp;
    qint32 accuracy;
    qint32 speed;
    qint32 heading;
    qint32 altitude;
    qint32 altitudeAccuracy;
};

Location::Private::Private():
    timestamp(0),
    accuracy(-1),
    speed(-1),
    heading(-1),
    altitude(0),
    altitudeAccuracy(-1)
{ }

Location::Private::Private(const Private &other):
    timestamp(other.timestamp),
    accuracy(other.accuracy),
    speed(other.speed),
    heading(other.heading),
    altitude(other.altitude),
    altitudeAccuracy(other.altitudeAccuracy)
{ }

Location::Location():
    Object(),
    KContacts::Geo(),
    d(new Private)
{ }

Location::Location(const Location & other):
    Object(other),
    KContacts::Geo(other),
    d(new Private(*(other.d)))
{ }

Location::Location(float latitude, float longitude):
    Object(),
    Geo(latitude, longitude),
    d(new Private)
{ }

Location::~Location()
{
    delete d;
}

qulonglong Location::timestamp() const
{
    return d->timestamp;
}

qint32 Location::accuracy() const
{
    return d->accuracy;
}

qint32 Location::speed() const
{
    return d->speed;
}

qint32 Location::heading() const
{
    return d->heading;
}

qint32 Location::altitude() const
{
    return d->altitude;
}

qint32 Location::altitudeAccuracy() const
{
    return d->altitudeAccuracy;
}

void Location::setTimestamp(qulonglong timestamp)
{
    d->timestamp = timestamp;
}

void Location::setAccuracy(qint32 accuracy)
{
    d->accuracy = accuracy;
}

void Location::setSpeed(qint32 speed)
{
    d->speed = speed;
}

void Location::setHeading(qint32 heading)
{
    d->heading = heading;
}

void Location::setAltitude(qint32 altitude)
{
    d->altitude = altitude;
}

void Location::setAltitudeAccuracy(qint32 altitudeAccuracy)
{
    d->altitudeAccuracy = altitudeAccuracy;
}
