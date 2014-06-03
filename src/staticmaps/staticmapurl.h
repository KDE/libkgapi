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

#ifndef LIBKGAPI2_STATICMAPURL_H
#define LIBKGAPI2_STATICMAPURL_H

#include <libkgapi2/staticmaps/staticmapmarker.h>
#include <libkgapi2/staticmaps/staticmappath.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QUrl>
#include <QtCore/QSize>

#include <KDE/KABC/Address>
#include <KDE/KABC/Geo>

namespace KGAPI2
{

/**
 * @brief A class to build a URL from StaticMapMarkers and StaticMapPaths to
 *        fetch a map tile.
 *
 * @author Jan Grulich <grulja@gmail.com>
 * @since 0.4
 */
class LIBKGAPI2_EXPORT StaticMapUrl
{
  public:

    enum LocationType {
        Undefined = -1,
        String,
        KABCAddress,
        KABCGeo
    };

    enum ImageFormat {
        PNG,
        PNG32,
        GIF,
        JPG,
        JPGBaseline
    };

    enum MapType {
        Roadmap,
        Satellite,
        Terrain,
        Hybrid
    };

    enum Scale {
        Normal = 1,
        TwiceBigger = 2
    };

    /**
     * @brief Constructs an empty StaticMapUrl
     */
    explicit StaticMapUrl();

    /**
     * @brief Constructs a new StaticMapUrl
     *
     * @param location Location of map center in QString
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the
     *               static map is using a sensor to determine the user's location.
     */
    StaticMapUrl(const QString &location, const QSize &size, quint32 zoom,
                 bool sensor);

    /**
     * @brief Constructs a new StaticMapUrl
     *
     * @param address Location of map center in KABC::Address
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the
     *               static map is using a sensor to determine the user's location.
     */
    StaticMapUrl(const KABC::Address &address, const QSize &size, quint32 zoom,
                 bool sensor);

    /**
     * @brief Constructs a new StaticMapUrl
     *
     * @param geo Location of map center in KABC::Geo
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the static map is
     * using a sensor to determine the user's location.
     */
    StaticMapUrl(const KABC::Geo &geo, const QSize &size, quint32 zoom,
                 bool sensor);

    /**
     * @brief Copy constructor
     */
    StaticMapUrl(const StaticMapUrl &other);

    /**
     * @brief Destructor
     */
    virtual ~StaticMapUrl();

    /**
     * @brief Returns in which format the location is stored.
     */
    LocationType locationType() const;

    /**
     * @brief Returns map image format
     */
    ImageFormat format() const;

    /**
     * @brief Sets map image format (default is PNG)
     *
     * @param format
     */
    void setFormat(const ImageFormat format);

    /**
     * @brief Returns whether map url is valid
     */
    bool isValid() const;

    /**
     * @brief Returns map center in QString
     *
     * Returns empty QString if location is not defined in QString
     */
    QString locationString() const;

    /**
     * @brief Defines center of the map
     *
     * @param location Location (required when no markers are present) of
     *                 center of the map in QString
     */
    void setLocation(const QString &location);

    /**
     * @brief Returns map center in KABC::Address
     *
     * Returns empty KABC::Address if location is not defined in KABC::Address
     */
    KABC::Address locationAddress() const;

    /**
     * @brief Defines center of the map
     *
     * @param address Location (required when no markers are present) of center
     *                of the map in KABC::Address
     */
    void setLocation(const KABC::Address & address);

    /**
     * @brief Returns map center in KABC::Geo
     *
     * Returns invalid KABC::Geo if location is not defined in KABC::Geo
     */
    KABC::Geo locationGeo() const;

    /**
     * @brief Defines center of the map
     *
     * @param geo Location (required when no markers are present) of center
     *            of the map in KABC::Geo
     */
    void setLocation(const KABC::Geo & geo);

    /**
     * @brief Returns type of map
     */
    MapType mapType() const;

    /**
     * @brief Sets type of map (roadmap by default)
     *
     * @param type
     */
    void setMapType(const MapType type);

    /**
     * @brief Returns list of markers
     *
     * Returns empty QList if no marker were added
     */
    QList<StaticMapMarker> markers() const;

    /**
     * @brief Adds marker to map
     *
     * @param marker
     */
    void setMarker(const StaticMapMarker & marker);

    /**
     * @brief Adds markers to map
     *
     * @param markers
     */
    void setMarkers(const QList<StaticMapMarker> & markers);

    /**
     * @brief  Returns list paths
     *
     * Returns empty QList if no path were added
     */
    QList<StaticMapPath> paths() const;

    /**
     * @brief Adds path to map
     *
     * @param path
     */
    void setPath(const StaticMapPath & path);

    /**
     * @brief Adds paths to map
     *
     * @param paths
     */
    void setPaths(QList<StaticMapPath> & paths);

    /**
     * @brief Returns size of map tile
     */
    QSize size() const;

    /**
     * @brief Sets size of requested map tile.
     *
     * @param size Maximum size is 640x640
     */
    void setSize(const QSize &size);

    /**
     * @brief Returns scale of map
     */
    Scale scale() const;

    /**
     * @brief Sets scale of map (default is 1)
     *
     * @param scale
     */
    void setScale(const Scale scale);

    /**
     * @brief Returns whether the application uses a sensor to determine user's
     *        location.
     */
    bool sensorUsed() const;

    /**
     * @brief Sets whether the application uses a sensor to determine the user's
     *        location.
     *
     * @param sensor
     */
    void setSensorUsed(const bool sensor);

    /**
     * @brief Returns visible area in QString
     *
     * Returns empty QString if visible area is not defined in QString
     */
    QString visibleLocationString() const;

    /**
     * @brief Sets visible location.
     *
     * You can use this instead of using center and zoom parameters.
     *
     * Specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to
     * ensure that certain features or map location are shown on the static map.
     *
     * @param location 
     */
    void setVisibleLocation(const QString & location);

    /**
     * @brief Returns visible area in KABC::Address
     *
     * Returns empty KABC::Address if visible area is not defined in KABC::Address
     */
    KABC::Address visibleLocationAddress() const;

    /**
     * @brief Sets visible location.
     *
     * You can use this instead of using center and zoom parameters.
     *
     * Visible specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to
     * ensure that certain features or map location are shown on the static map.
     *
     * @param address
     */
    void setVisibleLocation(const KABC::Address & address);

    /**
     * @brief Returns visible area in KABC::Geo
     *
     * Returns empty KABC::Geo if visible area is not defined in KABC::Geo
     */
    KABC::Geo visibleLocationGeo() const;

    /**
     * @brief Sets visible location.
     *
     * You can use this instead of using center and zoom parameters.
     *
     * Visible specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to
     * ensure that certain features or map location are shown on the static map.
     *
     * @param geo
     */
    void setVisibleLocation(const KABC::Geo & geo);

    /**
     * @brief Returns type of visible location
     */
    LocationType visibleLocationType() const;

    /**
     * @brief Returns zoom level of map
     *
     * Returns -1 if zoom level is not defined
     */
    qint8 zoomLevel() const;

    /**
     * @brief Sets zoom level of the map
     *
     * @param zoom Zoom parameter can be in range 0 to 21 where 21 is maximum
     *             zoom level.
     */
    void setZoomLevel(const quint32 zoom);

    /**
     * @brief Returns constructed url from all defined parameters
     */
    QUrl url() const;

    /**
     * @brief Assignment operator
     */
    StaticMapUrl& operator=(const StaticMapUrl &other);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_STATICMAPURL_H
