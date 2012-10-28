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

/**
 * DrivePermission contains a permission for a file.
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/permissions">Permissions</a>
 */
class LIBKGAPI_EXPORT DrivePermission: public KGAPI::Object
{
public:
    typedef QSharedPointer< DrivePermission > Ptr;
    typedef QList< DrivePermission > List;
    
    DrivePermission();
    DrivePermission(const DrivePermission& other);
    virtual ~DrivePermission();
    
    /**
     * Returns the id of the permission.
     */
    QString id();
    
    /**
     * Sets the id of the permission.
     */
    void setId(const QString& id);
    
    /**
     * Returns a link back to this permission.
     */
    QString selfLink();
    
    /**
     * Sets a link back to this permission.
     */
    void setSelfLink(const QString& selfLink);
    
    /**
     * Returns the name of this permission.
     */
    QString name();
    
    /**
     * Sets the name of this permission.
     */
    void setName(const QString& name);
    
    /**
     * Returns the primary role for this user.
     * Allowed values:
     * <ul>
     *  <li>owner</li>
     *  <li>reader</li>
     *  <li>writer</li>
     * </ul>
     */
    QString role();
    
    /**
     * Sets the primary role for this user.
     * Allowed values:
     * <ul>
     *  <li>owner</li>
     *  <li>reader</li>
     *  <li>writer</li>
     * </ul>
     */
    void setRole(const QString& role);
    
    /**
     * Returns additional roles for this user. Only commenter is currently allowed.
     */
    QStringList additionalRoles();
    
    /**
     * Sets additional roles for this user. Only commenter is currently allowed.
     */
    void setAdditionalRoles(const QStringList& additionalRoles);
    
    /**
     * Returns the account type.
     * Allowed values:
     * <ul>
     *  <li>user</li>
     *  <li>group</li>
     *  <li>domain</li>
     *  <li>anyone</li>
     * </ul>
     */
    QString type();
    
    /**
     * Sets the account type.
     * Allowed values:
     * <ul>
     *  <li>user</li>
     *  <li>group</li>
     *  <li>domain</li>
     *  <li>anyone</li>
     * </ul>
     */
    void setType(const QString& type);
    
    /**
     * Returns the authkey parameter required for this permission.
     */
    QString authKey();
    
    /**
     * Sets the authkey parameter required for this permission.
     */
    void setAuthKey(const QString& authKey);
    
    /**
     * Returns whether the link is required for this permission.
     */
    QVariant withLink();
    
    /**
     * Sets whether the link is required for this permission.
     */
    void setWithLink(const QVariant& withLink);
    
    /**
     * Returns a link to the profile photo, if available.
     */
    QString photoLink();
    
    /**
     * Sets a link to the profile photo, if available.
     */
    void setPhotoLink(const QString& photoLink);
    
    /**
     * Returns the email address or domain name for the entity.
     * This is not populated in responses.
     * You can use the alias "me" as the value for this property to refer to the current authorized user.
     */
    QString value();
    
    /**
     * Sets the email address or domain name for the entity.
     * This is not populated in responses.
     * You can use the alias "me" as the value for this property to refer to the current authorized user.
     */
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
