/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
        << Q_UINT64_C(1340366314362)
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
