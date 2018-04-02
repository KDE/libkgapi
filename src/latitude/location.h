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
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LIBKGAPI2_OBJECTS_LOCATION_H
#define LIBKGAPI2_OBJECTS_LOCATION_H

#include "object.h"
#include "types.h"
#include "kgapilatitude_export.h"

#include <KContacts/Geo>

#include <QSharedPointer>

namespace KGAPI2
{

/**
 * @brief Represents a single record about geographical location provided
 *        by Google Latitude service.
 *
 * @author Jan Grulich <grulja@gmail.com>
 * @since 0.4
 */
class KGAPILATITUDE_DEPRECATED_EXPORT Location : public KGAPI2::Object,
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
    ~Location() override;

    /**
     * @brief Timestamp of when this location has been recorded.
     *
     * Also serves as location unique ID
     */
    qulonglong timestamp() const;

    /**
     * @brief Sets timestamp of this location.
     */
    void setTimestamp(qulonglong timestamp);

    /**
     * @brief Returns accuracy of the latitude and longitude in meters.
     *
     * @return Returns -1 when accuracy is not defined.
     */
    qint32 accuracy() const;

    /**
     * @brief Sets accuracy of this location.
     */
    void setAccuracy(qint32 accuracy);

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
    void setSpeed(qint32 speed);

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
    void setHeading(qint32 heading);

    /**
     * @brief Returns altitude of this location.
     *
     * @return Returns 0 when altitude is not defined.
     */
    qint32 altitude() const;

    /**
     * @brief Sets altitude of this location.
     */
    void setAltitude(qint32 altitude);

    /**
     * @brief Returns altitude accuracy.
     *
     * @return Returns -1 when altitude accuracy is not defined.
     */
    qint32 altitudeAccuracy() const;

    /**
     * @brief Sets altitude accuracy of this location.
     */
    void setAltitudeAccuracy(qint32 altitudeAccuracy);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LOCATION_H
