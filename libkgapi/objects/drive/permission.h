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

#ifndef LIBKGAPI_OBJECTS_DRIVEPERMISSION_H
#define LIBKGAPI_OBJECTS_DRIVEPERMISSION_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace KGAPI
{

namespace Objects
{

class DrivePermissionPrivate;

class LIBKGAPI_EXPORT DrivePermission: public KGAPI::Object
{
public:
    typedef QSharedPointer< DrivePermission > Ptr;
    typedef QList< DrivePermission > List;
    
    DrivePermission();
    DrivePermission(const DrivePermission& other);
    virtual ~DrivePermission();
    
    QString id();
    void setId(const QString& id);
    
    QString selfLink();
    void setSelfLink(const QString& selfLink);
    
    QString name();
    void setName(const QString& name);
    
    QString role();
    void setRole(const QString& role);
    
    QStringList additionalRoles();
    void setAdditionalRoles(const QStringList& additionalRoles);
    
    QString type();
    void setType(const QString& type);
    
    QString authKey();
    void setAuthKey(const QString& authKey);
    
    bool withLink();
    void setWithLink(const bool& withLink);
    
    QString photoLink();
    void setPhotoLink(const QString& photoLink);
    
    QString value();
    void setValue(const QString& value);

private:
    DrivePermissionPrivate* d;
};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::DrivePermission::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::DrivePermission*)
Q_DECLARE_METATYPE(KGAPI::Objects::DrivePermission::List)

#endif // LIBKGAPI_OBJECTS_DRIVEPERMISSION_H
