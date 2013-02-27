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


#ifndef LIBKGAPI_OBJECTS_LOCATION_H
#define LIBKGAPI_OBJECTS_LOCATION_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <KDE/KABC/Geo>

#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class LIBKGAPI_EXPORT_DEPRECATED Location : public KGAPI::Object, public KABC::Geo
{
  public:
    typedef QSharedPointer<Location> Ptr;

    Location();
    Location(const Location &other);
    Location(float latitude, float longitude);

    virtual ~Location();

    /**
     * Returns timestamp of this location which is also the location unique ID.
     */
    qulonglong timestamp() const;

    /**
     * Returns accuracy of the latitude and longitude in meters.
     * Returns -1 when accuracy is not defined.
     */
    qint32 accuracy() const;

    /**
     * Returns ground speed of the user at the time this location was recorded.
     * Returns -1 when speed is not defined.
     */
    qint32 speed() const;

    /**
     * Returns direction of travel of the user when this location was recorded.
     * Returns -1 when heading is not defined.
     */
    qint32 heading() const;

    /**
     * Returns altitude of this location.
     * Returns 0 when altitude is not defined.
     */
    qint32 altitude() const;

    /**
     * Returns altitude accuracy.
     * Returns -1 when altitude accuracy is not defined.
     */
    qint32 altitudeAccuracy() const;

    /**
     * Sets timestamp of this location.
     */
    void setTimestamp(const qulonglong &timestamp);

    /**
     * Sets accuracy of this location.
     */
    void setAccuracy(const qint32 &accuracy);

    /**
     * Sets speed of this location.
     */
    void setSpeed(const qint32 &speed);

    /**
     * Sets heading of this location.
     */
    void setHeading(const qint32 &heading);

    /**
     * Sets altitude of this location.
     */
    void setAltitude(const qint32 &altitude);

    /**
     * Sets altitude accuracy of this location.
     */
    void setAltitudeAccuracy(const qint32 &altitudeAccuracy);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Objects

} // namespace KGAPI

#endif // LOCATION_H
