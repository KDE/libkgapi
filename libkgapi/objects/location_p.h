/*
    Copyright 2012  Jan Grulich <grulja@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIKBGAPI_OBJECTS_LOCATION_P_H
#define LIBKGAPI_OBJECTS_LOCATION_P_H

#include <QtCore/QString>

#include <KDateTime>

namespace KGAPI
{

namespace Objects
{

class LocationPrivate
{
public:
    LocationPrivate();
    LocationPrivate(const LocationPrivate &other);
    ~LocationPrivate() { };

    qulonglong timestamp;
    qint32 accuracy;
    qint32 speed;
    qint32 heading;
    qint32 altitude;
    qint32 altitudeAccuracy;

};

} /* namespace Objects */

} /* namespace KGAPI */

#endif // LIBKGAPI_OBJECTS_LOCATION_P_H
