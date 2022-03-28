/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <grulja@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "staticmapurl.h"

#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN StaticMapUrl::Private
{
public:
    Private();
    Private(const Private &other);

    void init(const Private &other);

    StaticMapUrl::LocationType locationType;
    StaticMapUrl::ImageFormat format;

    QString locationString;
    KContacts::Address locationAddress;
    KContacts::Geo locationGeo;

    StaticMapUrl::MapType maptype;
    QList<StaticMapMarker> markers;
    QList<StaticMapPath> paths;
    StaticMapUrl::Scale scale;
    bool sensor;
    QSize size;

    QString visibleString;
    KContacts::Address visibleAddress;
    KContacts::Geo visibleGeo;
    StaticMapUrl::LocationType visibleLocationType;

    qint32 zoom;
};

StaticMapUrl::Private::Private()
    : locationType(StaticMapUrl::Undefined)
    , format(StaticMapUrl::PNG)
    , maptype(StaticMapUrl::Roadmap)
    , scale(StaticMapUrl::Normal)
    , sensor(false)
    , visibleLocationType(StaticMapUrl::Undefined)
    , zoom(-1)
{
}

StaticMapUrl::Private::Private(const Private &other)
{
    init(other);
}

void StaticMapUrl::Private::init(const StaticMapUrl::Private &other)
{
    locationType = other.locationType;
    format = other.format;
    locationString = other.locationString;
    locationAddress = other.locationAddress;
    locationGeo = other.locationGeo;
    maptype = other.maptype;
    markers = other.markers;
    paths = other.paths;
    scale = other.scale;
    sensor = other.sensor;
    size = other.size;
    visibleString = other.visibleString;
    visibleAddress = other.visibleAddress;
    visibleGeo = other.visibleGeo;
    visibleLocationType = other.visibleLocationType;
    zoom = other.zoom;
}

StaticMapUrl::StaticMapUrl()
    : d(new Private)
{
}

StaticMapUrl::StaticMapUrl(const StaticMapUrl &other)
    : d(new Private(*(other.d)))
{
}

StaticMapUrl::~StaticMapUrl()
{
    delete d;
}

StaticMapUrl &StaticMapUrl::operator=(const StaticMapUrl &other)
{
    if (&other == this) {
        return *this;
    }

    d->init(*(other.d));
    return *this;
}

StaticMapUrl::StaticMapUrl(const QString &location, const QSize &size, quint32 zoom, bool sensor)
    : d(new Private)
{
    setLocation(location);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KContacts::Address &address, const QSize &size, quint32 zoom, bool sensor)
    : d(new Private)
{
    setLocation(address);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
}

StaticMapUrl::StaticMapUrl(const KContacts::Geo &geo, const QSize &size, quint32 zoom, bool sensor)
    : d(new Private)
{
    setLocation(geo);
    setSize(size);
    setZoomLevel(zoom);
    setSensorUsed(sensor);
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
        for (const StaticMapPath &path : std::as_const(d->paths)) {
            if (!path.isValid()) {
                maOrPa = false;
            }
        }
    } else {
        for (const StaticMapMarker &marker : std::as_const(d->markers)) {
            if (!marker.isValid()) {
                maOrPa = false;
            }
        }
    }

    if (maOrPa) {
        if ((d->locationType == Undefined || d->zoom == -1) && (d->visibleLocationType == Undefined)) {
            return false;
        }
    }

    return !(d->size.isEmpty());
}

QString StaticMapUrl::locationString() const
{
    return d->locationString;
}

void StaticMapUrl::setLocation(const QString &location)
{
    d->locationString = location;
    d->locationType = String;
    d->locationAddress.clear();
    d->locationGeo.setLatitude(91);
    d->locationGeo.setLongitude(181);
}

KContacts::Address StaticMapUrl::locationAddress() const
{
    return d->locationAddress;
}

void StaticMapUrl::setLocation(const KContacts::Address &address)
{
    d->locationAddress = address;
    d->locationType = KABCAddress;
    d->locationString.clear();
    d->locationGeo.setLatitude(91);
    d->locationGeo.setLongitude(181);
}

KContacts::Geo StaticMapUrl::locationGeo() const
{
    return d->locationGeo;
}

void StaticMapUrl::setLocation(const KContacts::Geo &geo)
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

QList<StaticMapMarker> StaticMapUrl::markers() const
{
    return d->markers;
}

void StaticMapUrl::setMarker(const StaticMapMarker &marker)
{
    QList<StaticMapMarker> markers;
    markers << marker;
    d->markers = markers;
}

void StaticMapUrl::setMarkers(const QList<StaticMapMarker> &markers)
{
    d->markers = markers;
}

QList<StaticMapPath> StaticMapUrl::paths() const
{
    return d->paths;
}

void StaticMapUrl::setPath(const StaticMapPath &path)
{
    QList<StaticMapPath> paths;
    paths << path;
    d->paths = paths;
}

void StaticMapUrl::setPaths(const QList<StaticMapPath> &paths)
{
    d->paths = paths;
}

QSize StaticMapUrl::size() const
{
    return d->size;
}

void StaticMapUrl::setSize(const QSize &size)
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

void StaticMapUrl::setVisibleLocation(const QString &location)
{
    d->visibleString = location;
    d->visibleLocationType = String;
    d->visibleAddress.clear();
    d->visibleGeo.setLatitude(911);
    d->visibleGeo.setLongitude(181);
}

KContacts::Address StaticMapUrl::visibleLocationAddress() const
{
    return d->locationAddress;
}

void StaticMapUrl::setVisibleLocation(const KContacts::Address &address)
{
    d->visibleAddress = address;
    d->visibleLocationType = KABCAddress;
    d->visibleString.clear();
    d->visibleGeo.setLatitude(911);
    d->visibleGeo.setLongitude(181);
}

KContacts::Geo StaticMapUrl::visibleLocationGeo() const
{
    return d->locationGeo;
}

void StaticMapUrl::setVisibleLocation(const KContacts::Geo &geo)
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
    QUrl url(QStringLiteral("http://maps.googleapis.com/maps/api/staticmap"));
    QUrlQuery query(url);

    if (d->locationType != Undefined) {
        QString param;

        switch (d->locationType) {
        case Undefined:
        case String:
            param = d->locationString;
            param.replace(QLatin1String(", "), QLatin1String(","));
            param.replace(QLatin1String(". "), QLatin1String("."));
            param.replace(QLatin1Char(' '), QLatin1Char('+'));
            query.addQueryItem(QStringLiteral("center"), param);
            break;
        case KABCAddress:
            param = d->locationAddress.formatted(KContacts::AddressFormatStyle::Postal);
            param.replace(QLatin1String(", "), QLatin1String(","));
            param.replace(QLatin1String(". "), QLatin1String("."));
            param.replace(QLatin1Char(' '), QLatin1Char('+'));
            param.replace(QLatin1Char('\n'), QLatin1Char(','));
            query.addQueryItem(QStringLiteral("center"), param);
            break;
        case KABCGeo:
            param = QString::number(d->locationGeo.latitude()) + QLatin1Char(',') + QString::number(d->locationGeo.longitude());
            query.addQueryItem(QStringLiteral("center"), param);
            break;
        }
    }

    if (d->zoom != -1) {
        query.addQueryItem(QStringLiteral("zoom"), QString::number(d->zoom));
    }

    if (!d->size.isEmpty()) {
        QString size = QString::number(d->size.width()) + QLatin1Char('x') + QString::number(d->size.height());
        query.addQueryItem(QStringLiteral("size"), size);
    }

    if (d->scale != Normal) {
        query.addQueryItem(QStringLiteral("scale"), QString::number(2));
    }
    if (d->format != PNG) {
        QString format;

        switch (d->format) {
        case PNG:
        case PNG32:
            format = QStringLiteral("png32");
            break;
        case GIF:
            format = QStringLiteral("gif");
            break;
        case JPG:
            format = QStringLiteral("jpg");
            break;
        case JPGBaseline:
            format = QStringLiteral("jpg-baseline");
            break;
        }

        query.addQueryItem(QStringLiteral("format"), format);
    }

    if (d->maptype != Roadmap) {
        QString maptype;

        switch (d->maptype) {
        case Roadmap:
        case Satellite:
            maptype = QStringLiteral("satellite");
            break;
        case Terrain:
            maptype = QStringLiteral("terrain");
            break;
        case Hybrid:
            maptype = QStringLiteral("hybrid");
            break;
        }

        query.addQueryItem(QStringLiteral("maptype"), maptype);
    }

    for (const StaticMapMarker &marker : std::as_const(d->markers)) {
        if (marker.isValid()) {
            query.addQueryItem(QStringLiteral("markers"), marker.toString());
        }
    }

    for (const StaticMapPath &path : std::as_const(d->paths)) {
        if (path.isValid()) {
            query.addQueryItem(QStringLiteral("path"), path.toString());
        }
    }

    if (d->visibleLocationType != Undefined) {
        QString param;

        switch (d->visibleLocationType) {
        case Undefined:
        case String:
            param = d->visibleString;
            param.replace(QLatin1String(", "), QLatin1String(","));
            param.replace(QLatin1String(". "), QLatin1String("."));
            param.replace(QLatin1Char(' '), QLatin1Char('+'));
            query.addQueryItem(QStringLiteral("visible"), param);
            break;
        case KABCAddress:
            param = d->visibleAddress.formatted(KContacts::AddressFormatStyle::Postal);
            param.replace(QLatin1String(", "), QLatin1String(","));
            param.replace(QLatin1String(". "), QLatin1String("."));
            param.replace(QLatin1Char(' '), QLatin1Char('+'));
            param.replace(QLatin1Char('\n'), QLatin1Char(','));
            query.addQueryItem(QStringLiteral("visible"), param);
            break;
        case KABCGeo:
            param = QString::number(d->visibleGeo.latitude()) + QLatin1Char(',') + QString::number(d->visibleGeo.longitude());
            query.addQueryItem(QStringLiteral("visible"), param);
            break;
        }
    }

    if (d->sensor) {
        query.addQueryItem(QStringLiteral("sensor"), QStringLiteral("true"));
    } else {
        query.addQueryItem(QStringLiteral("sensor"), QStringLiteral("false"));
    }

    url.setQuery(query);
    return url;
}
