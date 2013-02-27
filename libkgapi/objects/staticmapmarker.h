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


#ifndef LIBKGAPI_OBJECTS_STATICMAPMARKER_H
#define LIBKGAPI_OBJECTS_STATICMAPMARKER_H

#include <libkgapi/libkgapi_export.h>

#include <QColor>

#include <KDE/KABC/Address>
#include <KDE/KABC/Geo>

namespace KGAPI
{

namespace Objects
{

/**
 * Represents marker with defined label, color, size and marker locations
 */
class LIBKGAPI_EXPORT_DEPRECATED StaticMapMarker
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
     * Constructs an empty marker
     */
    StaticMapMarker();

    /**
     * Constructs a new marker
     *
     * @param address Location in QString where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}.
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QString & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * Constructs a new marker
     *
     * @param address Location in KABC::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KABC::Address & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * Constructs a new marker
     *
     * @param address Location in KABC::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the markers
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     * @param color Color of marker
     */
    explicit StaticMapMarker(const KABC::Geo & address, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * Constructs a new marker
     *
     * @param locations Locations in QString where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QStringList & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * Constructs a new marker
     *
     * @param locations Locations in KABC::Address where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QList<KABC::Address> & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    /**
     * Constructs a new marker
     *
     * @param locations Locations in KABC::Geo where marker will be visible
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     * @param color Color of marker
     */
    explicit StaticMapMarker(const QList<KABC::Geo> & locations, const QChar & label = QChar(),
                             const MarkerSize size = Normal, const QColor & color = Qt::red);

    StaticMapMarker(const StaticMapMarker & other);

    ~StaticMapMarker();

    /**
     * Returns in which format is location saved.
     */
    LocationType locationType() const;

    /**
     * Returns color of marker
     */
    QColor color() const;

    /**
     * Sets color of marker
     *
     * @param color Color for marker
     */
    void setColor(const QColor & color);

    /**
     * Returns if marker is valid. It means that marker needs defined location
     */
    bool isValid() const;

    /**
     * Returns label of marker
     */
    QChar label() const;

    /**
     * Sets label of marker
     *
     * @param label Specifies a single uppercase alphanumeric character from the set {A-Z, 0-9}
     * 		    which will be displayed in the marker
     */
    void setLabel(const QChar & label);

    /**
     * Returns locations in QString
     */
    QStringList locationsString() const;

    /**
     * Sets one location for marker
     *
     * @param location Location for marker in QString
     */
    void setLocation(const QString & location);

    /**
     * Sets locations for marker
     *
     * @param locations Locations for marker in QString
     */
    void setLocations(const QStringList & locations);

    /**
     * Returns locations in KABC::Address
     */
    QList<KABC::Address> locationsAddress() const;

    /**
     * Sets one location for marker
     *
     * @param location Location for marker in KABC::Address
     */
    void setLocation(const KABC::Address & location);

    /**
     * Sets locations for marker
     *
     * @param locations Locations for marker in KABC::Address
     */
    void setLocations(const QList<KABC::Address> & locations);

    /**
     * Returns locations in KABC::Geo
     */
    QList<KABC::Geo> locationsGeo() const;

    /**
     * Sets one location for marker
     *
     * @param location Location for marker in KABC::Geo
     */
    void setLocation(const KABC::Geo & location);

    /**
     * Sets locations for marker
     *
     * @param locations Locations for marker in KABC::Geo
     */
    void setLocations(const QList<KABC::Geo> & locations);

    /**
     * Returns all locations and markers preferences in format to URL query.
     */
    QString toString() const;

    /**
     * Returns size of marker
     */
    MarkerSize size() const;

    /**
     * Sets size of marker
     *
     * @param size Specifies the size of marker from the set {tiny, small, mid, normal}
     */
    void setSize(const MarkerSize size);


    /**
     * Assignment operator
     */
    StaticMapMarker& operator=(const StaticMapMarker &other);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} /* namespace Objects */

} /* namespace KGAPI */


#endif // LIBKGAPI_OBJECTS_STATICMAPMARKER_H
