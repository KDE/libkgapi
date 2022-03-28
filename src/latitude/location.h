/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "kgapilatitude_export.h"
#include "object.h"
#include "types.h"

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
class KGAPILATITUDE_DEPRECATED_EXPORT Location : public KGAPI2::Object, public KContacts::Geo
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
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2
