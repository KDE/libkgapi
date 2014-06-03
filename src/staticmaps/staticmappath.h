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


#ifndef LIBKGAPI2_STATICMAPPATH_H
#define LIBKGAPI2_STATICMAPPATH_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtGui/QColor>

#include <KDE/KABC/Address>
#include <KDE/KABC/Geo>

namespace KGAPI2
{

/**
 * @brief Represents path with defined locations, weight, color and color for 
 *        filled area
 *
 * @author Jan Grulich <grulja@gmail.com>
 * @since 0.4
 */
class LIBKGAPI2_EXPORT StaticMapPath
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
     * @param locations The path locations in KABC::Address
     * @param weight The thickness of the path in pixels
     * @param color Color of the path
     * @param fillColor The color of filled area
     */
    explicit StaticMapPath(const QList<KABC::Address> & locations, const quint8 weight = 5,
                           const QColor & color = Qt::blue, const QColor & fillColor = QColor());

    /**
     * @brief Constructs a new path
     *
     * @param locations The path locations in KABC::Geo
     * @param weight The thickness of the path in pixels
     * @param color The color of the path
     * @param fillColor The color of filled area
     */
    explicit StaticMapPath(const QList<KABC::Geo> & locations, const quint8 weight = 5,
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
     * @brief Returns whther the path is valid.
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
     * @brief  Returns locations in KABC::Address
     *
     * Returns empty list if is not defined
     */
    QList<KABC::Address> locationsAddress() const;

    /**
     * @brief  Sets locations for path
     *
     * @param locations Locations for path in KABC::Address
     */
    void setLocations(const QList<KABC::Address> & locations);

    /**
     * @brief Returns locations in KABC::Geo
     *
     * Returns empty list if is not defined
     */
    QList<KABC::Geo> locationsGeo() const;

    /**
     * @brief Sets locations for path
     *
     * @param locations Locations for path in KABC::Geo
     */
    void setLocations(const QList<KABC::Geo> & locations);

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

#endif // LIBKGAPI2_STATICMAPPATH_H
