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

#include "objectstaticmapurltests.h"
#include "objects/staticmapurl.h"

using namespace KGAPI;
using namespace Objects;

Q_DECLARE_METATYPE(StaticMapUrl::ImageFormat);
Q_DECLARE_METATYPE(StaticMapUrl::MapType);
Q_DECLARE_METATYPE(StaticMapUrl::Scale)
Q_DECLARE_METATYPE(StaticMapMarker::MarkerSize);
Q_DECLARE_METATYPE(KABC::Address);
Q_DECLARE_METATYPE(KABC::Geo);
Q_DECLARE_METATYPE(QList<KABC::Address>);
Q_DECLARE_METATYPE(QList<KABC::Geo>);

void ObjectStaticMapUrlTests::testBasicUrl_data()
{
    QTest::addColumn<QString>("center");
    QTest::addColumn<quint8>("zoom");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<StaticMapUrl::ImageFormat>("format");
    QTest::addColumn<StaticMapUrl::MapType>("maptype");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QTest::newRow("map1")
        << "Uncovice 83, Litovel"
        << (quint8) 14
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << StaticMapUrl::PNG
        << StaticMapUrl::Roadmap
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=Uncovice+83,Litovel&zoom=14&size=640x640&sensor=false";

    QTest::newRow("map2")
        << "Gregrova 5, Olomouc"
        << (quint8) 15
        << QSize(800, 640)
        << (StaticMapUrl::Scale) 2
        << StaticMapUrl::GIF
        << StaticMapUrl::Satellite
        << true
        << "http://maps.googleapis.com/maps/api/staticmap?center=Gregrova+5,Olomouc&zoom=15&size=800x640&scale=2&format=gif&maptype=satellite&sensor=true";
}

void ObjectStaticMapUrlTests::testBasicUrl()
{
    StaticMapUrl url;

    QFETCH(QString, center);
    QFETCH(quint8, zoom);
    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(StaticMapUrl::ImageFormat, format);
    QFETCH(StaticMapUrl::MapType, maptype);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setLocation(center);
    url.setZoomLevel(zoom);
    url.setSize(size);
    url.setScale(scale);
    url.setFormat(format);
    url.setMapType(maptype);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testBasicUrlAddress_data()
{
    QTest::addColumn<KABC::Address>("center");
    QTest::addColumn<quint8>("zoom");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<StaticMapUrl::ImageFormat>("format");
    QTest::addColumn<StaticMapUrl::MapType>("maptype");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    KABC::Address addr1;
    addr1.setLocality("Litovel");
    addr1.setStreet("Uncovice");

    QTest::newRow("map1")
        << addr1
        << (quint8) 14
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << StaticMapUrl::PNG32
        << StaticMapUrl::Terrain
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=Uncovice,Litovel&zoom=14&size=640x640&format=png32&maptype=terrain&sensor=false";

    KABC::Address addr2;
    addr2.setLocality("Olomouc");
    addr2.setStreet("Gregrova");

    QTest::newRow("map2")
        << addr2
        << (quint8) 16
        << QSize(800, 640)
        << (StaticMapUrl::Scale) 2
        << StaticMapUrl::JPG
        << StaticMapUrl::Hybrid
        << true
        << "http://maps.googleapis.com/maps/api/staticmap?center=Gregrova,Olomouc&zoom=16&size=800x640&scale=2&format=jpg&maptype=hybrid&sensor=true";

}

void ObjectStaticMapUrlTests::testBasicUrlAddress()
{
    StaticMapUrl url;

    QFETCH(KABC::Address, center);
    QFETCH(quint8, zoom);
    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(StaticMapUrl::ImageFormat, format);
    QFETCH(StaticMapUrl::MapType, maptype);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setLocation(center);
    url.setZoomLevel(zoom);
    url.setSize(size);
    url.setScale(scale);
    url.setFormat(format);
    url.setMapType(maptype);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testBasicUrlGeo_data()
{
    QTest::addColumn<KABC::Geo>("center");
    QTest::addColumn<quint8>("zoom");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<StaticMapUrl::ImageFormat>("format");
    QTest::addColumn<StaticMapUrl::MapType>("maptype");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QTest::newRow("map1")
        << KABC::Geo(49.7014, 17.0756)
        << (quint8) 14
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << StaticMapUrl::JPGBaseline
        << StaticMapUrl::Roadmap
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=49.7014,17.0756&zoom=14&size=640x640&format=jpg-baseline&sensor=false";

    QTest::newRow("map2")
        << KABC::Geo(49.5953, 17.2518)
        << (quint8) 13
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 2
        << StaticMapUrl::GIF
        << StaticMapUrl::Satellite
        << true
        << "http://maps.googleapis.com/maps/api/staticmap?center=49.5953,17.2518&zoom=13&size=640x640&scale=2&format=gif&maptype=satellite&sensor=true";
}

void ObjectStaticMapUrlTests::testBasicUrlGeo()
{
    StaticMapUrl url;

    QFETCH(KABC::Geo, center);
    QFETCH(quint8, zoom);
    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(StaticMapUrl::ImageFormat, format);
    QFETCH(StaticMapUrl::MapType, maptype);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setLocation(center);
    url.setZoomLevel(zoom);
    url.setSize(size);
    url.setScale(scale);
    url.setFormat(format);
    url.setMapType(maptype);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testMarkers_data()
{
    QTest::addColumn<QString>("center");
    QTest::addColumn<quint8>("zoom");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<StaticMapUrl::ImageFormat>("format");
    QTest::addColumn<StaticMapUrl::MapType>("maptype");
    QTest::addColumn<QStringList>("markerlocations");
    QTest::addColumn<QChar>("markerlabel");
    QTest::addColumn<QColor>("markercolor");
    QTest::addColumn<StaticMapMarker::MarkerSize>("markersize");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QStringList list1;
    list1 << "Uncovice, Litovel";
    list1 << "Brezove, Litovel";
    list1 << "Rozvadovice, Litovel";

    QStringList list2;
    list2 << "Gregrova, Olomouc";
    list2 << "Generala Piky, Olomouc";
    list2 << "Palackeho, Olomouc";

    QTest::newRow("map1")
        << "Litovel"
        << (quint8) 12
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << StaticMapUrl::PNG
        << StaticMapUrl::Roadmap
        << list1
        << QChar('T')
        << QColor(Qt::blue)
        << StaticMapMarker::Middle
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=Litovel&zoom=12&size=640x640&markers=size:mid|color:0x0000ff|label:T|Uncovice,Litovel|Brezove,Litovel|Rozvadovice,Litovel&sensor=false";

    QTest::newRow("map2")
        << "Olomouc"
        << (quint8) 11
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 2
        << StaticMapUrl::GIF
        << StaticMapUrl::Hybrid
        << list2
        << QChar()
        << QColor(Qt::red)
        << StaticMapMarker::Normal
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=Olomouc&zoom=11&size=640x640&scale=2&format=gif&maptype=hybrid&markers=Gregrova,Olomouc|Generala+Piky,Olomouc|Palackeho,Olomouc&sensor=false";

}

void ObjectStaticMapUrlTests::testMarkers()
{
    StaticMapUrl url;

    QFETCH(QString, center);
    QFETCH(quint8, zoom);
    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(StaticMapUrl::ImageFormat, format);
    QFETCH(StaticMapUrl::MapType, maptype);
    QFETCH(QStringList, markerlocations);
    QFETCH(QChar, markerlabel);
    QFETCH(QColor, markercolor);
    QFETCH(StaticMapMarker::MarkerSize, markersize);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setLocation(center);
    url.setZoomLevel(zoom);
    url.setSize(size);
    url.setScale(scale);
    url.setFormat(format);
    url.setMapType(maptype);
    url.setMarker(StaticMapMarker(markerlocations, markerlabel, markersize, markercolor));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testMarkersAddress_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QList< KABC::Address> >("markerlocations");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QList<KABC::Address> list1;
    KABC::Address addr1;
    addr1.setLocality("Litovel");
    addr1.setStreet("Uncovice");
    KABC::Address addr2;
    addr2.setLocality("Litovel");
    addr2.setStreet("Brezove");
    KABC::Address addr3;
    addr3.setLocality("Litovel");
    addr3.setStreet("Rozvadovice");

    list1 << addr1;
    list1 << addr2;
    list1 << addr3;

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list1
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&markers=Uncovice,Litovel|Brezove,Litovel|Rozvadovice,Litovel&sensor=false";
}

void ObjectStaticMapUrlTests::testMarkersAddress()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QList<KABC::Address>, markerlocations);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setMarker(StaticMapMarker(markerlocations));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testMarkersGeo_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QList< KABC::Geo> >("markerlocations");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QList<KABC::Geo> list1;
    list1 << KABC::Geo(49.7014, 17.0756);
    list1 << KABC::Geo(49.5953, 17.2518);

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list1
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&markers=49.7014,17.0756|49.5953,17.2518&sensor=false";
}

void ObjectStaticMapUrlTests::testMarkersGeo()
{
StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QList<KABC::Geo>, markerlocations);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setMarker(StaticMapMarker(markerlocations));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testPath_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QStringList>("pathlocations");
    QTest::addColumn<quint8>("weight");
    QTest::addColumn<QColor>("color");
    QTest::addColumn<QColor>("fillcolor");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QStringList list1;
    list1 << "Uncovice, Litovel";
    list1 << "Brezove, Litovel";
    list1 << "Rozvadovice, Litovel";

    QStringList list2;
    list2 << "Gregrova, Olomouc";
    list2 << "Generala Piky, Olomouc";
    list2 << "Palackeho, Olomouc";

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list1
        << (quint8) 3
        << QColor(Qt::red)
        << QColor(Qt::yellow)
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&path=color:0xff0000|weight:3|fillcolor:0xffff00|Uncovice,Litovel|Brezove,Litovel|Rozvadovice,Litovel&sensor=false";

    QTest::newRow("map2")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list2
        << (quint8) 5
        << QColor(Qt::blue)
        << QColor()
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&path=Gregrova,Olomouc|Generala+Piky,Olomouc|Palackeho,Olomouc&sensor=false";
}

void ObjectStaticMapUrlTests::testPath()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QStringList, pathlocations);
    QFETCH(quint8, weight);
    QFETCH(QColor, color);
    QFETCH(QColor, fillcolor);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setPath(StaticMapPath(pathlocations, weight, color, fillcolor));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testPathAddress_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QList< KABC::Address> >("pathlocations");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QList<KABC::Address> list1;
    KABC::Address addr1;
    addr1.setLocality("Litovel");
    addr1.setStreet("Uncovice");
    KABC::Address addr2;
    addr2.setLocality("Litovel");
    addr2.setStreet("Brezove");
    KABC::Address addr3;
    addr3.setLocality("Litovel");
    addr3.setStreet("Rozvadovice");

    list1 << addr1;
    list1 << addr2;
    list1 << addr3;

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list1
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&path=Uncovice,Litovel|Brezove,Litovel|Rozvadovice,Litovel&sensor=false";
}

void ObjectStaticMapUrlTests::testPathAddress()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QList<KABC::Address>, pathlocations);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setPath(StaticMapPath(pathlocations));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testPathGeo_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QList< KABC::Geo> >("pathlocations");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QList<KABC::Geo> list1;
    list1 << KABC::Geo(49.7014, 17.0756);
    list1 << KABC::Geo(49.5953, 17.2518);

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << list1
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&path=49.7014,17.0756|49.5953,17.2518&sensor=false";
}

void ObjectStaticMapUrlTests::testPathGeo()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QList<KABC::Geo>, pathlocations);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setPath(StaticMapPath(pathlocations));
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testVisibleLocation_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<QString>("visiblearea");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << "Uncovice, Litovel"
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=Uncovice,Litovel&sensor=false";

    QTest::newRow("map2")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << "Gregrova, Olomouc"
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=Gregrova,Olomouc&sensor=false";
}

void ObjectStaticMapUrlTests::testVisibleLocation()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(QString, visiblearea);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setVisibleLocation(visiblearea);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testVisibleLocationAddress_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<KABC::Address>("visiblearea");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    KABC::Address addr1;
    addr1.setLocality("Litovel");
    addr1.setStreet("Uncovice");

    KABC::Address addr2;
    addr2.setLocality("Olomouc");
    addr2.setStreet("Gregrova");

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << addr1
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=Uncovice,Litovel&sensor=false";

    QTest::newRow("map2")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << addr2
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=Gregrova,Olomouc&sensor=false";
}

void ObjectStaticMapUrlTests::testVisibleLocationAddress()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(KABC::Address, visiblearea);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setVisibleLocation(visiblearea);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testVisibleLocationGeo_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<KABC::Geo>("visiblearea");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QTest::newRow("map1")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << KABC::Geo(49.7014, 17.0756)
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=49.7014,17.0756&sensor=false";

    QTest::newRow("map2")
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 1
        << KABC::Geo(49.5953, 17.2518)
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?size=640x640&visible=49.5953,17.2518&sensor=false";
}

void ObjectStaticMapUrlTests::testVisibleLocationGeo()
{
    StaticMapUrl url;

    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(KABC::Geo, visiblearea);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setSize(size);
    url.setScale(scale);
    url.setVisibleLocation(visiblearea);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}

void ObjectStaticMapUrlTests::testComplicatedUrl_data()
{
    QTest::addColumn<QString>("center");
    QTest::addColumn<quint8>("zoom");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<StaticMapUrl::Scale>("scale");
    QTest::addColumn<StaticMapUrl::ImageFormat>("format");
    QTest::addColumn<StaticMapUrl::MapType>("maptype");
    QTest::addColumn<QList<KABC::Geo> >("markerlocations");
    QTest::addColumn<QChar>("markerlabel");
    QTest::addColumn<QColor>("markercolor");
    QTest::addColumn<StaticMapMarker::MarkerSize>("markersize");
    QTest::addColumn<QStringList>("pathlocations");
    QTest::addColumn<quint8>("pathweight");
    QTest::addColumn<QColor>("pathcolor");
    QTest::addColumn<QColor>("pathfillcolor");
    QTest::addColumn<KABC::Address>("visiblearea");
    QTest::addColumn<bool>("sensor");
    QTest::addColumn<QString>("result");

    QList<KABC::Geo> markers;
    markers << KABC::Geo(49.7014, 17.0756);
    markers << KABC::Geo(49.5953, 17.2518);

    QStringList path;
    path << "Litovel";
    path << "Olomouc";
    path << "Sternberk";
    path << "Unicov";
    path << "Litovel";

    KABC::Address visible;
    visible.setLocality("Olomouc");

    QTest::newRow("map1")
        << "Olomoucky kraj"
        << (quint8) 10
        << QSize(640, 640)
        << (StaticMapUrl::Scale) 2
        << StaticMapUrl::JPG
        << StaticMapUrl::Terrain
        << markers
        << QChar('T')
        << QColor(Qt::yellow)
        << StaticMapMarker::Middle
        << path
        << (quint8) 3
        << QColor(Qt::black)
        << QColor(Qt::green)
        << visible
        << false
        << "http://maps.googleapis.com/maps/api/staticmap?center=Olomoucky+kraj&zoom=10&size=640x640&scale=2&format=jpg&maptype=terrain&markers=size:mid|color:0xffff00|label:T|49.7014,17.0756|49.5953,17.2518&path=color:0x000000|weight:3|fillcolor:0x00ff00|Litovel|Olomouc|Sternberk|Unicov|Litovel&visible=Olomouc&sensor=false";
}

void ObjectStaticMapUrlTests::testComplicatedUrl()
{
    StaticMapUrl url;

    QFETCH(QString, center);
    QFETCH(quint8, zoom);
    QFETCH(QSize, size);
    QFETCH(StaticMapUrl::Scale, scale);
    QFETCH(StaticMapUrl::ImageFormat, format);
    QFETCH(StaticMapUrl::MapType, maptype);
    QFETCH(QList<KABC::Geo>, markerlocations);
    QFETCH(QChar, markerlabel);
    QFETCH(QColor, markercolor);
    QFETCH(StaticMapMarker::MarkerSize, markersize);
    QFETCH(QStringList, pathlocations);
    QFETCH(quint8, pathweight);
    QFETCH(QColor, pathcolor);
    QFETCH(QColor, pathfillcolor);
    QFETCH(KABC::Address, visiblearea);
    QFETCH(bool, sensor);
    QFETCH(QString, result);

    url.setLocation(center);
    url.setZoomLevel(zoom);
    url.setSize(size);
    url.setScale(scale);
    url.setFormat(format);
    url.setMapType(maptype);
    url.setMarker(StaticMapMarker(markerlocations, markerlabel, markersize, markercolor));
    url.setPath(StaticMapPath(pathlocations, pathweight, pathcolor, pathfillcolor));
    url.setVisibleLocation(visiblearea);
    url.setSensorUsed(sensor);

    QCOMPARE(url.url().toString(), result);
}


QTEST_KDEMAIN(ObjectStaticMapUrlTests, NoGUI);
