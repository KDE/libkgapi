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


#ifndef OBJECTSTATICMAPURLTESTS_H
#define OBJECTSTATICMAPURLTESTS_H

#include <qtest_kde.h>

class ObjectStaticMapUrlTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testBasicUrl_data();
    void testBasicUrl();
    void testBasicUrlAddress_data();
    void testBasicUrlAddress();
    void testBasicUrlGeo_data();
    void testBasicUrlGeo();
    void testMarkers_data();
    void testMarkers();
    void testMarkersAddress_data();
    void testMarkersAddress();
    void testMarkersGeo_data();
    void testMarkersGeo();
    void testPath_data();
    void testPath();
    void testPathAddress_data();
    void testPathAddress();
    void testPathGeo_data();
    void testPathGeo();
    void testVisibleLocation_data();
    void testVisibleLocation();
    void testVisibleLocationAddress_data();
    void testVisibleLocationAddress();
    void testVisibleLocationGeo_data();
    void testVisibleLocationGeo();
    void testComplicatedUrl_data();
    void testComplicatedUrl();
};

#endif // OBJECTSTATICMAPURLTESTS_H
