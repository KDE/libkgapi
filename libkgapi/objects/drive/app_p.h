/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

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

#ifndef LIBKGAPI_OBJECTS_DRIVEAPP_P_H
#define LIBKGAPI_OBJECTS_DRIVEAPP_P_H

#include <QtCore/QString>

namespace KGAPI
{
    
namespace Objects
{

class DriveAppPrivate_Icon
{
public:
    QString category;
    int size;
    QString iconUrl;
};

class DriveAppPrivate
{
public:
    // need that because nested classes can't be forward-declared
    typedef DriveAppPrivate_Icon Icon;
    
    DriveAppPrivate() { };
    DriveAppPrivate(const DriveAppPrivate &other);
    ~DriveAppPrivate() { };

    QString id;
    QString name;
    QString objectType;
    bool supportsCreate;
    bool supportsImport;
    bool installed;
    bool authorized;
    bool useByDefault;
    QString productUrl;
    QStringList primaryMimeTypes;
    QStringList secondaryMimeTypes;
    QStringList primaryFileExtensions;
    QStringList secondaryFileExtensions;
    QList<DriveApp::Icon> icons;
};

} /* namespace Objects */
    
} /* namespace KGAPI */
    
#endif // LIBKGAPI_OBJECTS_DRIVEAPP_P_H