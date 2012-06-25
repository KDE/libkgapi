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


#include "location.h"
#include "location_p.h"

using namespace KGAPI::Objects;

LocationData::LocationData():
    timestamp(0),
    accuracy(-1),
    speed(-1),
    heading(-1),
    altitude(0),
    altitudeAccuracy(-1)
{ }

LocationData::LocationData(const LocationData &other):
    QSharedData(other),
    timestamp(other.timestamp),
    accuracy(other.accuracy)
{ }

Location::Location():
    KGAPI::Object(),
    d(new LocationData)
{ }

Location::Location(const Location & other):
    KGAPI::Object(other),
    KABC::Geo(other),
    d(other.d)
{ }

Location::Location(float latitude, float longitude):
    Geo(latitude, longitude),
    d(new LocationData)
{ }

Location::~Location()
{ }

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

void Location::setTimestamp(const qulonglong & timestamp)
{
    d->timestamp = timestamp;
}

void Location::setAccuracy(const qint32 & accuracy)
{
    d->accuracy = accuracy;
}

void Location::setSpeed(const qint32 & speed)
{
    d->speed = speed;
}

void Location::setHeading(const qint32 & heading)
{
    d->heading = heading;
}

void Location::setAltitude(const qint32 & altitude)
{
    d->altitude = altitude;
}

void Location::setAltitudeAccuracy(const qint32 & altitudeAccuracy)
{
    d->altitudeAccuracy = altitudeAccuracy;
}
