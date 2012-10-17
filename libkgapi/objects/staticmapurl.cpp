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

#include "staticmapurl.h"
#include "staticmapurl_p.h"
#include "kurl.h"
#include <QDebug>

using namespace KGAPI::Objects;

StaticMapUrlPrivate::StaticMapUrlPrivate():
    locationType(StaticMapUrl::Undefined),
    format(StaticMapUrl::PNG),
    maptype(StaticMapUrl::Roadmap),
    scale(StaticMapUrl::Normal),
    sensor(false),
    visibleLocationType(StaticMapUrl::Undefined),
    zoom(-1)
{ }

StaticMapUrlPrivate::StaticMapUrlPrivate(const StaticMapUrlPrivate & other):
    locationType(other.locationType),
    format(other.format),
    locationString(other.locationString),
    locationAddress(other.locationAddress),
    locationGeo(other.locationGeo),
    maptype(other.maptype),
    markers(other.markers),
    paths(other.paths),
    scale(other.scale),
    sensor(other.sensor),
    size(other.size),
    visibleString(other.visibleString),
    visibleAddress(other.visibleAddress),
    visibleGeo(other.visibleGeo),
    visibleLocationType(other.visibleLocationType),
    zoom(other.zoom)
{ }

StaticMapUrlPrivate::~StaticMapUrlPrivate()
{ }

StaticMapUrl::StaticMapUrl():
    d(new StaticMapUrlPrivate)
{ }

StaticMapUrl::StaticMapUrl(const StaticMapUrl& other):
    d(new StaticMapUrlPrivate(*(other.d)))
{ }

StaticMapUrl::StaticMapUrl(const QString& location, const QSize size,
			   const quint32 zoom, const bool sensor):
    d(new StaticMapUrlPrivate)
{
    setLocation(location);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KABC::Address& address, const QSize size,
			   const quint32 zoom, const bool sensor):
    d(new StaticMapUrlPrivate)
{
    setLocation(address);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KABC::Geo& geo, const QSize size,
                           const quint32 zoom, const bool sensor):
    d(new StaticMapUrlPrivate)
{
    setLocation(geo);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::~StaticMapUrl()
{
    delete d;
}

StaticMapUrl::LocationType StaticMapUrl::locationType() const
{
    return d->locationType;
}

StaticMapUrl::ImageFormat StaticMapUrl::format() const
{
    return d->format;
}

void StaticMapUrl::setFormat(const StaticMapUrl::ImageFormat format)
{
    d->format = format;
}

bool StaticMapUrl::isValid() const
{
    bool maOrPa = true;

    if (d->markers.isEmpty()) {
        Q_FOREACH(const StaticMapPath & path, d->paths) {
            if (!path.isValid())
                maOrPa = false;
        }
    } else {

        Q_FOREACH(const StaticMapMarker & marker, d->markers) {
            if (!marker.isValid())
                maOrPa = false;
        }

    }

    if (maOrPa) {

        if ((d->locationType == Undefined || d->zoom == -1) &&
                (d->visibleLocationType == Undefined))
            return false;

    }

    return !(d->size.isEmpty());
}

QString StaticMapUrl::locationString() const
{
    return d->locationString;
}

void StaticMapUrl::setLocation(const QString& location)
{
    d->locationString = location;
    d->locationType = String;
    d->locationAddress.clear();
    d->locationGeo.setLatitude(91);
    d->locationGeo.setLongitude(181);
}

KABC::Address StaticMapUrl::locationAddress() const
{
    return d->locationAddress;
}

void StaticMapUrl::setLocation(const KABC::Address& address)
{
    d->locationAddress = address;
    d->locationType = KABCAddress;
    d->locationString.clear();
    d->locationGeo.setLatitude(91);
    d->locationGeo.setLongitude(181);
}

KABC::Geo StaticMapUrl::locationGeo() const
{
    return d->locationGeo;
}

void StaticMapUrl::setLocation(const KABC::Geo& geo)
{
    d->locationGeo = geo;
    d->locationType = KABCGeo;
    d->locationString.clear();
    d->locationAddress.clear();
}

StaticMapUrl::MapType StaticMapUrl::mapType() const
{
    return d->maptype;
}

void StaticMapUrl::setMapType(const StaticMapUrl::MapType type)
{
    d->maptype = type;
}

QList< StaticMapMarker > StaticMapUrl::markers() const
{
    return d->markers;
}

void StaticMapUrl::setMarker(const StaticMapMarker& marker)
{
    QList<StaticMapMarker> markers;
    markers << marker;
    d->markers = markers;
}

void StaticMapUrl::setMarkers(const QList< StaticMapMarker >& markers)
{
    d->markers = markers;
}

QList<StaticMapPath> StaticMapUrl::paths() const
{
    return d->paths;
}

void StaticMapUrl::setPath(const StaticMapPath & path)
{
    QList<StaticMapPath> paths;
    paths << path;
    d->paths = paths;
}

void StaticMapUrl::setPaths(QList< StaticMapPath >& paths)
{
    d->paths = paths;
}

QSize StaticMapUrl::size() const
{
    return d->size;
}

void StaticMapUrl::setSize(const QSize& size)
{
    d->size = size;
}

StaticMapUrl::Scale StaticMapUrl::scale() const
{
    return d->scale;
}

void StaticMapUrl::setScale(const Scale scale)
{
    d->scale = scale;
}

bool StaticMapUrl::sensorUsed() const
{
    return d->sensor;
}

void StaticMapUrl::setSensorUsed(const bool sensor)
{
    d->sensor = sensor;
}

QString StaticMapUrl::visibleLocationString() const
{
    return d->visibleString;
}

void StaticMapUrl::setVisibleLocation(const QString & location)
{
    d->visibleString = location;
    d->visibleLocationType = String;
    d->visibleAddress.clear();
    d->visibleGeo.setLatitude(911);
    d->visibleGeo.setLongitude(181);
}

KABC::Address StaticMapUrl::visibleLocationAddress() const
{
    return d->locationAddress;
}

void StaticMapUrl::setVisibleLocation(const KABC::Address & address)
{
    d->visibleAddress = address;
    d->visibleLocationType = KABCAddress;
    d->visibleString.clear();
    d->visibleGeo.setLatitude(911);
    d->visibleGeo.setLongitude(181);
}

KABC::Geo StaticMapUrl::visibleLocationGeo() const
{
    return d->locationGeo;
}

void StaticMapUrl::setVisibleLocation(const KABC::Geo & geo)
{
    d->visibleGeo = geo;
    d->visibleLocationType = KABCGeo;
    d->visibleString.clear();
    d->visibleAddress.clear();
}

StaticMapUrl::LocationType StaticMapUrl::visibleLocationType() const
{
    return d->visibleLocationType;
}

qint8 StaticMapUrl::zoomLevel() const
{
    return d->zoom;
}

void StaticMapUrl::setZoomLevel(const quint32 zoom)
{
    d->zoom = zoom;
}

QUrl StaticMapUrl::url() const
{
    KUrl url("http://maps.googleapis.com/maps/api/staticmap");

    if (d->locationType != Undefined) {
        QString param;

        switch (d->locationType) {
	case Undefined:
        case String:
            param = d->locationString;
            param = param.replace(", ", ",");
            param = param.replace(". ", ".");
            param = param.replace(" ", "+");
            url.addQueryItem("center", param);
            break;
        case KABCAddress:
            param = d->locationAddress.formattedAddress();
            param = param.replace(", ", ",");
            param = param.replace(". ", ".");
            param = param.replace(" ", "+");
            param = param.replace("\n",",");
            url.addQueryItem("center", param);
            break;
        case KABCGeo:
            param = QString::number(d->locationGeo.latitude()) + "," +
                    QString::number(d->locationGeo.longitude());
            url.addQueryItem("center", param);
            break;
        }
    }

    if (d->zoom != -1)
        url.addQueryItem("zoom", QString::number(d->zoom));

    if (!d->size.isEmpty()) {
        QString size = QString::number(d->size.width()) + "x" +
                       QString::number(d->size.height());
        url.addQueryItem("size", size);
    }

    if (d->scale != Normal)
        url.addQueryItem("scale", QString::number(2));
    if (d->format != PNG) {
        QString format;

        switch (d->format) {
        case PNG:
        case PNG32:
            format = "png32";
            break;
        case GIF:
            format = "gif";
            break;
        case JPG:
            format = "jpg";
            break;
        case JPGBaseline:
            format = "jpg-baseline";
            break;
        }

        url.addQueryItem("format", format);
    }

    if (d->maptype != Roadmap) {
        QString maptype;

        switch (d->maptype) {
        case Roadmap:
        case Satellite:
            maptype = "satellite";
            break;
        case Terrain:
            maptype = "terrain";
            break;
        case Hybrid:
            maptype = "hybrid";
            break;
        }

        url.addQueryItem("maptype", maptype);
    }

    Q_FOREACH(const StaticMapMarker & marker, d->markers) {

        if (marker.isValid())
            url.addQueryItem("markers", marker.toString());
    }

    Q_FOREACH(const StaticMapPath & path, d->paths) {

        if (path.isValid())
            url.addQueryItem("path", path.toString());

    }

    if (d->visibleLocationType != Undefined) {

        QString param;

        switch (d->visibleLocationType) {
	case Undefined:
        case String:
            param = d->visibleString;
            param = param.replace(", ", ",");
            param = param.replace(". ", ".");
            param = param.replace(" ", "+");
            url.addQueryItem("visible", param);
            break;
        case KABCAddress:
            param = d->visibleAddress.formattedAddress();
            param = param.replace(", ", ",");
            param = param.replace(". ", ".");
            param = param.replace(" ", "+");
            param = param.replace("\n",",");
            url.addQueryItem("visible", param);
            break;
        case KABCGeo:
            param = QString::number(d->visibleGeo.latitude()) + "," +
                    QString::number(d->visibleGeo.longitude());
            url.addQueryItem("visible", param);
            break;
        }
    }

    if (d->sensor)
        url.addQueryItem("sensor", "true");
    else
        url.addQueryItem("sensor", "false");

    return url;
}
