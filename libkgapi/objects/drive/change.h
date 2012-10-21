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
    
    /**
     * Returns the id of the change.
     */
    qulonglong id();
    
    /**
     * Sets the id of the change.
     */
    void setId(const qulonglong& id);
    
    /**
     * Returns the id of the file associated with this change.
     */
    QString fileId();
    
    /**
     * Sets the id of the file associated with this change.
     */
    void setFileId(const QString& fileId);
    
    /**
     * Returns a link back to this change.
     */
    QString selfLink();
    
    /**
     * Sets the link back to this change.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns whether this file has been deleted.
     */
    bool deleted();
    
    /**
     * Sets whether this file has been deleted.
     */
    void setDeleted(const bool& deleted);
    
    /**
     * Returns the updated state of the file.
     * Present if the file has not been deleted.
     */
    DriveFile::Ptr file();
    
    /**
     * Sets the updated state of the file.
     * Present if the file has not been deleted.
     */
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
