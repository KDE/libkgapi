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

#ifndef LIBKGAPI_OBJECTS_DRIVECHANGE_H
#define LIBKGAPI_OBJECTS_DRIVECHANGE_H

#include <libkgapi/objects/drive/file.h>

#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DriveChangePrivate;

class LIBKGAPI_EXPORT DriveChange: public KGAPI::Object
{
public:
    typedef QSharedPointer< DriveChange > Ptr;
    typedef QList< DriveChange > List;
    
    DriveChange();
    DriveChange(const DriveChange& other);
    virtual ~DriveChange();
    
    qulonglong id();
    void setId(const qulonglong& id);
    
    QString fileId();
    void setFileId(const QString& fileId);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    bool deleted();
    void setDeleted(const bool& deleted);
    
    DriveFile::Ptr file();
    void setFile(const DriveFile::Ptr& file);

private:
    DriveChangePrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DriveChange::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveChange*)
Q_DECLARE_METATYPE(KGAPI::Objects::DriveChange::List)

#endif // LIBKGAPI_OBJECTS_DRIVECHANGE_H
