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

#ifndef LIBKGAPI_OBJECTS_STATICMAPURL_H
#define LIBKGAPI_OBJECTS_STATICMAPURL_H

#include <libkgapi/objects/staticmapmarker.h>
#include <libkgapi/objects/staticmappath.h>
#include <libkgapi/libkgapi_export.h>

#include <QUrl>
#include <QSize>

#include <KDE/KABC/Address>
#include <KDE/KABC/Geo>

namespace KGAPI
{

namespace Objects
{

class LIBKGAPI_EXPORT_DEPRECATED StaticMapUrl
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
     * Constructs an empty StaticMapUrl
     */
    StaticMapUrl();

    /**
     * Constructs a new StaticMapUrl
     *
     * @param location Location of map center in QString
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the static map is
     * using a sensor to determine the user's location.
     */
    StaticMapUrl(const QString &location, const QSize size, const quint32 zoom, const bool sensor);

    /**
     * Constructs a new StaticMapUrl
     *
     * @param address Location of map center in KABC::Address
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the static map is
     * using a sensor to determine the user's location.
     */
    StaticMapUrl(const KABC::Address & address, const QSize size, const quint32 zoom, const bool sensor);

    /**
     * Constructs a new StaticMapUrl
     *
     * @param geo Location of map center in KABC::Geo
     * @param size Requested size of the map image
     * @param zoom Zoom level of map
     * @param sensor Sensor specifies whether the application requesting the static map is
     * using a sensor to determine the user's location.
     */
    StaticMapUrl(const KABC::Geo & geo, const QSize size, const quint32 zoom, const bool sensor);

    StaticMapUrl(const StaticMapUrl &other);

    ~StaticMapUrl();

    LocationType locationType() const;

    /**
     * Returns map image format
     */
    ImageFormat format() const;

    /**
     * Sets map image format
     *
     * @param format Format (optional) of map could be png (default),png32, gif, jpg, jpg-baseline
     */
    void setFormat(const ImageFormat format);

    /**
     * Returns whether map url is valid
     */
    bool isValid() const;

    /**
     * Returns map center in QString
     * Returns empty QString if location is not defined in QString
     */
    QString locationString() const;

    /**
     * Defines center of the map
     *
     * @param location Location (required if markers not present) of center in QString
     */
    void setLocation(const QString &location);

    /**
     * Returns map center in KABC::Address
     * Returns empty KABC::Address if location is not defined in KABC::Address
     */
    KABC::Address locationAddress() const;

    /**
     * Defines center of the map
     *
     * @param address Location (required if markers not present) of center in KABC::Address
     */
    void setLocation(const KABC::Address & address);

    /**
     * Returns map center in KABC::Geo
     * Returns invalid KABC::Geo if location is not defined in KABC::Geo
     */
    KABC::Geo locationGeo() const;

    /**
     * Defines center of the map
     *
     * @param geo Location (required if markers not present) of center in KABC::Geo
     */
    void setLocation(const KABC::Geo & geo);

    /**
     * Returns maptype of map
     */
    MapType mapType() const;

    /**
     * Sets maptype of map
     *
     * @param type Maptype (optional) could be roadmap (default), satellite, terrain, hybrid
     */
    void setMapType(const MapType type);

    /**
     * Returns list of markers
     * Returns empty QList if none marker were not added
     */
    QList<StaticMapMarker> markers() const;

    /**
     * Adds marker to map
     *
     * @param marker Marker (optional) which will be added to map
     */
    void setMarker(const StaticMapMarker & marker);

    /**
     * Adds markers to map
     *
     * @param markers List (optional) of markers
     */
    void setMarkers(const QList<StaticMapMarker> & markers);

    /**
     * Returns list paths
     * Returns empty QList if none path were not added
     */
    QList<StaticMapPath> paths() const;

    /**
     * Adds path to map
     *
     * @param path Path (optional) which will be visible in map
     */
    void setPath(const StaticMapPath & path);

    /**
     * Adds paths to map
     *
     * @param paths Paths (optional) which will be visible in map
     */
    void setPaths(QList<StaticMapPath> & paths);

    /**
     * Returns size of map
     */
    QSize size() const;

    /**
     * Sets size of map in QSize
     *
     * @param size Size (required) of map in QSize. Maximum size is 640x640
     */
    void setSize(const QSize &size);

    /**
     * Returns scale of map
     */
    Scale scale() const;

    /**
     * Sets scale of map
     *
     * @param scale Scale (optional) of map could be 1 (default) or 2 (returns twice as large picture)
     */
    void setScale(const Scale scale);

    /**
     * Returns if the application uses a sensor to determine the user's location.
     */
    bool sensorUsed() const;

    /**
     * Sets if the application uses a sensor to determine the user's location.
     *
     * @param sensor Sensor (required) specifies whether the application requesting the static map is
     * using a sensor to determine the user's location.
     */
    void setSensorUsed(const bool sensor);

    /**
     * Returns visible area in QString
     * Returns empty QString if visible area is not defined in QString
     */
    QString visibleLocationString() const;

    /**
     * Sets visible location. You can use this instead of using center and zoom parameters.
     *
     * @param location Visible (optional) specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to ensure that certain features or map location
     * are shown on the static map.
     */
    void setVisibleLocation(const QString & location);

    /**
     * Returns visible area in KABC::Address
     * Returns empty KABC::Address if visible area is not defined in KABC::Address
     */
    KABC::Address visibleLocationAddress() const;

    /**
     * Sets visible location. You can use this instead of using center and zoom parameters.
     *
     * @param address Visible (optional) specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to ensure that certain features or map location
     * are shown on the static map.
     */
    void setVisibleLocation(const KABC::Address & address);

    /**
     * Returns visible area in KABC::Geo
     * Returns empty KABC::Geo if visible area is not defined in KABC::Geo
     */
    KABC::Geo visibleLocationGeo() const;

    /**
     * Sets visible location. You can use this instead of using center and zoom parameters.
     *
     * @param geo Visible (optional) specifies location that should remain visible on the map, though
     * no markers or other indicators will be displayed. Use this parameter to ensure that certain features or map location
     * are shown on the static map.
     */
    void setVisibleLocation(const KABC::Geo & geo);
    /**
     * Returns type of visible location
     */
    LocationType visibleLocationType() const;

    /**
     * Returns zoom level of map
     * Returns -1 if zoom level is not defined
     */
    qint8 zoomLevel() const;

    /**
     * Sets zoom level of the map
     *
     * @param zoom Zoom (required if markers not present) parameter could be in range 0 to 21 where 21 is maximum zoom level.
     */
    void setZoomLevel(const quint32 zoom);

    /**
     * Returns constructed url from all defined parameters
     */
    QUrl url() const;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} /* namespace Objects */

} /* namespace KGAPI */

#endif // LIBKGAPI_OBJECTS_STATICMAPURL_H
