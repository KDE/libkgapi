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

#ifndef LIBKGAPI2_DRIVEPERMISSION_H
#define LIBKGAPI2_DRIVEPERMISSION_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QString>
#include <QtCore/QStringList>

namespace KGAPI2
{

/**
 * @brief DrivePermission contains a permission for a file.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/permissions">Permissions</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class LIBKGAPI2_EXPORT DrivePermission
{

  public:
    enum Role {
        UndefinedRole = -1,
        OwnerRole = 0,
        ReaderRole = 1,
        WriterRole = 2
    };

    enum Type {
        UndefinedType = -1,
        TypeUser = 0,
        TypeGroup = 1,
        TypeDomain = 2,
        TypeAnyone = 3
    };

    explicit DrivePermission();
    explicit DrivePermission(const DrivePermission &other);
    virtual ~DrivePermission();

    /**
     * @brief Returns the id of the permission.
     */
    QString id();

    /**
     * @brief Sets the id of the permission.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns a link back to this permission.
     */
    QString selfLink();

    /**
     * @brief Sets a link back to this permission.
     *
     * @param selfLink
     */
    void setSelfLink(const QString &selfLink);

    /**
     * @brief Returns the name of this permission.
     */
    QString name();

    /**
     * @brief Sets the name of this permission.
     *
     * @param name
     */
    void setName(const QString &name);

    /**
     * @brief Returns the primary role for this user.
     */
    DrivePermission::Role role();

    /**
     * @brief Sets the primary role for this user.
     */
    void setRole(DrivePermission::Role role);

    /**
     * @brief Returns additional roles for this user. Only commenter is currently allowed.
     */
    QStringList additionalRoles();

    /**
     * @brief Sets additional roles for this user. Only commenter is currently allowed.
     *
     * @param additionalRoles
     */
    void setAdditionalRoles(const QStringList &additionalRoles);

    /**
     * @brief Returns the account type.
     */
    DrivePermission::Type type();

    /**
     * @brief Sets the account type.
     *
     * @param type
     */
    void setType(DrivePermission::Type type);

    /**
     * @brief Returns the authkey parameter required for this permission.
     */
    QString authKey();

    /**
     * @brief Sets the authkey parameter required for this permission.
     *
     * @param authKey
     */
    void setAuthKey(const QString &authKey);

    /**
     * @brief Returns whether the link is required for this permission.
     */
    bool withLink();

    /**
     * @brief Sets whether the link is required for this permission.
     *
     * @param withLink
     */
    void setWithLink(bool withLink);

    /**
     * @brief Returns a link to the profile photo, if available.
     */
    QString photoLink();

    /**
     * @brief Sets a link to the profile photo, if available.
     *
     * @param photoLink
     */
    void setPhotoLink(const QString &photoLink);

    /**
     * @brief Returns the email address or domain name for the entity.
     *
     * This is not populated in responses.
     * You can use the alias "me" as the value for this property to refer to the
     * current authorized user.
     */
    QString value();

    /**
     * @brief Sets the email address or domain name for the entity.
     *
     * This is not populated in responses.
     * You can use the alias "me" as the value for this property to refer to the
     * current authorized user.
     *
     * @param value
     */
    void setValue(const QString &value);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEPERMISSION_H
