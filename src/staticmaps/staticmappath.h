/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#pragma once

#include "kgapimaps_export.h"

#include <QColor>

#include <KContacts/Address>
#include <KContacts/Geo>

namespace KGAPI2
{

/**
 * @brief Represents path with defined locations, weight, color and color for 
 *        filled area
 *
 * @author Jan Grulich <grulja@gmail.com>
 * @since 0.4
 */
class KGAPIMAPS_EXPORT StaticMapPath
{
  public:

    enum LocationType {
        Undefined = -1,
        String,
        KABCAddress,
        KABCGeo,
    };

    /**
     * @brief Constructs an empty path
     */
    explicit StaticMapPath();

    /**
     * @brief Constructs a new path
     *
     * @param locations The path locations in QString
     * @param weight The thickness of the path in pixels
     * @param color The color of the path
     * @param fillColor The color of filled area
     */
    explicit StaticMapPath(const QStringList & locations, const quint8 weight = 5,
                           const QColor & color = Qt::blue, const QColor & fillColor = QColor());

    /**
     * @brief Constructs a new path
     *
     * @param locations The path locations in KContacts::Address
     * @param weight The thickness of the path in pixels
     * @param color Color of the path
     * @param fillColor The color of filled area
     */
    explicit StaticMapPath(const KContacts::Address::List &locations, const quint8 weight = 5,
                           const QColor & color = Qt::blue, const QColor & fillColor = QColor());

    /**
     * @brief Constructs a new path
     *
     * @param locations The path locations in KContacts::Geo
     * @param weight The thickness of the path in pixels
     * @param color The color of the path
     * @param fillColor The color of filled area
     */
    explicit StaticMapPath(const QList<KContacts::Geo> & locations, const quint8 weight = 5,
                           const QColor & color = Qt::blue, const QColor & fillColor = QColor());

    /**
     * @brief Copy constructor
     */
    StaticMapPath(const StaticMapPath &other);

    /**
     * @brief Destructor
     */
    ~StaticMapPath();

    /**
     * @brief Location type
     */
    LocationType locationType() const;

    /**
     * @brief Returns the color of path
     */
    QColor color() const;

    /**
     * @brief Sets color of the path
     *
     * @param color Color for path
     */
    void setColor(const QColor & color);

    /**
     * @brief Returns the color of filled area
     */
    QColor fillColor() const;

    /**
     * @brief Sets color for filled area in path
     *
     * @param color The color for filled area
     */
    void setFillColor(const QColor & color);

    /**
     * @brief Returns whether the path is valid.
     *
     * This means that path has at least two locations
     */
    bool isValid() const;

    /**
     * @brief Returns locations in QString
     *
     * Returns empty list if is not defined
     */
    QStringList locationsString() const;

    /**
     * @brief Sets locations for path
     *
     * @param locations Locations for path in QString
     */
    void setLocations(const QStringList & locations);

    /**
     * @brief  Returns locations in KContacts::Address
     *
     * Returns empty list if is not defined
     */
    KContacts::Address::List locationsAddress() const;

    /**
     * @brief  Sets locations for path
     *
     * @param locations Locations for path in KContacts::Address
     */
    void setLocations(const KContacts::Address::List & locations);

    /**
     * @brief Returns locations in KContacts::Geo
     *
     * Returns empty list if is not defined
     */
    QList<KContacts::Geo> locationsGeo() const;

    /**
     * @brief Sets locations for path
     *
     * @param locations Locations for path in KContacts::Geo
     */
    void setLocations(const QList<KContacts::Geo> & locations);

    /**
     * @brief Returns all locations and path preferences in format to URL query.
     */
    QString toString() const;

    /**
     * @brief Returns weight of the path
     */
    quint8 weight() const;

    /**
     * @brief Sets weight of the path
     *
     * @param weight The thickness of the path in pixels
     */
    void setWeight(const quint8 weight);

    /**
     * @brief Assignment operator
     */
    StaticMapPath& operator=(const StaticMapPath &other);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

