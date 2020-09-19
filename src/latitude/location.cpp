/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "location.h"

using namespace KGAPI2;

class Q_DECL_HIDDEN Location::Private
{
  public:
    Private();
    Private(const Private &other);

    qulonglong timestamp = 0;
    qint32 accuracy = -1;
    qint32 speed = -1;
    qint32 heading = -1;
    qint32 altitude = 0;
    qint32 altitudeAccuracy = -1;
};

Location::Private::Private()
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
