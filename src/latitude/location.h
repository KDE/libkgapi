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


#ifndef LIBKGAPI2_OBJECTS_LOCATION_H
#define LIBKGAPI2_OBJECTS_LOCATION_H

#include "object.h"
#include "types.h"
#include "kgapilatitude_export.h"

#include <KContacts/Geo>

#include <QtCore/QSharedPointer>

namespace KGAPI2
{

/**
 * @brief Represents a single record about geographical location provided
 *        by Google Latitude service.
 *
 * @author Jan Grulich <grulja@gmail.com>
 * @since 0.4
 */
class KGAPILATITUDE_EXPORT Location : public KGAPI2::Object,
                                      public KContacts::Geo
{
  public:

    /**
     * @brief Constructor
     */
    Location();

    /**
     * @brief Constructor
     *
     * @param latitude
     * @param longitude
     */
    Location(float latitude, float longitude);

    /**
     * @brief Copy constructor
     */
    Location(const Location &other);

    /**
     * @brief Destructor
     */
    virtual ~Location();

    /**
     * @brief Timestamp of when this location has been recorded.
     *
     * Also serves as location unique ID
     */
    qulonglong timestamp() const;

    /**
     * @brief Sets timestamp of this location.
     */
    void setTimestamp(const qulonglong &timestamp);

    /**
     * @brief Returns accuracy of the latitude and longitude in meters.
     *
     * @return Returns -1 when accuracy is not defined.
     */
    qint32 accuracy() const;

    /**
     * @brief Sets accuracy of this location.
     */
    void setAccuracy(const qint32 &accuracy);

    /**
     * @brief Returns ground speed of the user at the time this location was
     *        recorded.
     *
     * @return Returns -1 when speed is not defined.
     */
    qint32 speed() const;

    /**
     * @brief Sets speed of this location.
     */
    void setSpeed(const qint32 &speed);

    /**
     * @brief Returns direction of travel of the user when this location was
     *        recorded.
     *
     * @return Returns -1 when heading is not defined.
     */
    qint32 heading() const;

    /**
     * @brief Sets heading of this location.
     */
    void setHeading(const qint32 &heading);

    /**
     * @brief Returns altitude of this location.
     *
     * @return Returns 0 when altitude is not defined.
     */
    qint32 altitude() const;

    /**
     * @brief Sets altitude of this location.
     */
    void setAltitude(const qint32 &altitude);

    /**
     * @brief Returns altitude accuracy.
     *
     * @return Returns -1 when altitude accuracy is not defined.
     */
    qint32 altitudeAccuracy() const;

    /**
     * @brief Sets altitude accuracy of this location.
     */
    void setAltitudeAccuracy(const qint32 &altitudeAccuracy);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LOCATION_H
