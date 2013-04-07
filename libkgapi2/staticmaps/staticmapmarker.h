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
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1Normal01 USA.
*/


#ifndef LIBKGAPI2_STATICMAPMARKER_H
#define LIBKGAPI2_STATICMAPMARKER_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtGui/QColor>

#include <KDE/KABC/Address>
#include <KDE/KABC/Geo>

namespace KGAPI2
{

/**
 * @brief Represents marker with defined label, color, size and markers
 *
 * @author: Jan Grulich <grulja@gmail.com>
 * @since: 0.4
 */
class LIBKGAPI2_EXPORT StaticMapMarker
{

  public:

    enum MarkerSize {
        Tiny,
        Small,
        Middle,
        Normal
    };

    enum LocationType {
        Undefined = -1,
        String,
        KABCAddress,
        KABCGeo
    };

    /**
     * @brief Constructs an empty marker
     */
    StaticMapMarker();

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in QString where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QString & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in KABC::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KABC::Address & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param address Location in KABC::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KABC::Geo & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QStringList & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param locations Locations in KABC::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QList<KABC::Address> & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Constructs a new marker
     *
     * @param locations Locations in KABC::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     * @param size Specifies the size of marker
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QList<KABC::Geo> & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * @brief Copy constructor
     */
    StaticMapMarker(const StaticMapMarker & other);

    /**
     * @brief Destructor
     */
    ~StaticMapMarker();

    /**
     * @brief Returns in which format is location saved.
     */
    LocationType locationType() const;

    /**
     * @brief Returns color of marker
     */
    QColor color() const;

    /**
     * @brief Sets color of marker
     *
     * @param color Color for marker
     */
    void setColor(const QColor & color);

    /**
     * @brief Returns if marker is valid. It means that marker needs defined location
     */
    bool isValid() const;

    /**
     * @brief Returns label of marker
     */
    QChar label() const;

    /**
     * @brief Sets label of marker
     *
     * @param label Specifies a single uppercase alphanumeric character from
     *              set {A-Z, 0-9} which will be displayed in the marker
     */
    void setLabel(const QChar & label);

    /**
     * @brief Returns locations in QString
     */
    QStringList locationsString() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in QString
     */
    void setLocation(const QString & location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in QString
     */
    void setLocations(const QStringList & locations);

    /**
     * @brief Returns locations in KABC::Address
     */
    QList<KABC::Address> locationsAddress() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in KABC::Address
     */
    void setLocation(const KABC::Address & location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in KABC::Address
     */
    void setLocations(const QList<KABC::Address> & locations);

    /**
     * @brief Returns locations in KABC::Geo
     */
    QList<KABC::Geo> locationsGeo() const;

    /**
     * @brief Sets one location for marker
     *
     * @param location Location for marker in KABC::Geo
     */
    void setLocation(const KABC::Geo & location);

    /**
     * @brief Sets locations for marker
     *
     * @param locations Locations for marker in KABC::Geo
     */
    void setLocations(const QList<KABC::Geo> & locations);

    /**
     * @brief Returns all locations and markers preferences in format to URL query.
     */
    QString toString() const;

    /**
     * @brief Returns size of marker
     */
    MarkerSize size() const;

    /**
     * @brief Sets size of marker
     *
     * @param size Specifies the size of marker
     */
    void setSize(const MarkerSize size);

    /**
     * @brief Assignment operator
     */
    StaticMapMarker& operator=(const StaticMapMarker &other);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2


#endif // LIBKGAPI2_STATICMAPMARKER_H
