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


#include "serviceslatitudetests.h"
#include "objects/location.h"
#include "services/latitude.h"

void ServicesLatitudeTests::testJSONParser_data()
{
    QTest::addColumn<qulonglong>("timestamp");
    QTest::addColumn<float>("latitude");
    QTest::addColumn<float>("longitude");
    QTest::addColumn<qint32>("accuracy");
    QTest::addColumn<qint32>("speed");
    QTest::addColumn<qint32>("heading");
    QTest::addColumn<qint32>("altitude");
    QTest::addColumn<qint32>("altitudeAccuracy");
    QTest::addColumn<QByteArray>("jsonData");

    QTest::newRow("location1")
        << (qulonglong) 1340366314362
        << (float) 49.6664083
        << (float) 17.1053383
        << 25
        << -1
        << -1
        << 0
        << -1
        << QByteArray("{\n"
           "    \"data\": {\n"
           "    \"timestampMs\": \"1340366314362\",\n"
           "    \"latitude\": 49.6664083,\n"
           "    \"longitude\": 17.1053383,\n"
           "    \"accuracy\": 25,\n"
           "    \"speed\": -1,\n"
           "    \"heading\": -1,\n"
           "    \"altitude\": 0,\n"
           "    \"altitudeAccuracy\": -1\n"
           " }"
           "}");
}

void ServicesLatitudeTests::testJSONParser()
{
    KGAPI::Objects::Location *location;
    KGAPI::Services::Latitude *service;

    QFETCH(qulonglong, timestamp);
    QFETCH(float, latitude);
    QFETCH(float, longitude);
    QFETCH(qint32, accuracy);
    QFETCH(qint32, speed);
    QFETCH(qint32, heading);
    QFETCH(qint32, altitude);
    QFETCH(qint32, altitudeAccuracy);
    QFETCH(QByteArray, jsonData);

    service = new KGAPI::Services::Latitude();

    location = dynamic_cast<KGAPI::Objects::Location*>(service->JSONToObject(jsonData));

    QCOMPARE(location->timestamp(), timestamp);
    QCOMPARE(location->latitude(), latitude);
    QCOMPARE(location->longitude(), longitude);
    QCOMPARE(location->accuracy(), accuracy);
    QCOMPARE(location->speed(), speed);
    QCOMPARE(location->heading(), heading);
    QCOMPARE(location->altitude(), altitude);
    QCOMPARE(location->altitudeAccuracy(), altitudeAccuracy);

    delete location;
    delete service;
}

QTEST_KDEMAIN(ServicesLatitudeTests, NoGUI);
