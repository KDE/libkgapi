/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

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

#ifndef LIBKGAPI2_DRIVEPERMISSION_H
#define LIBKGAPI2_DRIVEPERMISSION_H

#include "object.h"
#include "types.h"
#include "file.h"
#include "kgapidrive_export.h"

#include <QString>
#include <QUrl>

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief Permission contains a permission for a file.
 *
 * Getters and setters' documentation is based on Google Drive's API v2 reference
 * @see <a href="https://developers.google.com/drive/v2/reference/permissions">Permissions</a>
 *
 * @since 2.0
 * @author Andrius da Costa Ribas <andriusmao@gmail.com>
 * @author Daniel Vrátil <dvratil@redhat.com>
 */
class KGAPIDRIVE_EXPORT Permission: public KGAPI2::Object
{

  public:
    enum Role {
        UndefinedRole = -1,
        OwnerRole = 0,
        ReaderRole = 1,
        WriterRole = 2,
        CommenterRole = 3,
    };

    enum Type {
        UndefinedType = -1,
        TypeUser = 0,
        TypeGroup = 1,
        TypeDomain = 2,
        TypeAnyone = 3
    };

    explicit Permission();
    explicit Permission(const Permission &other);
    ~Permission() override;

    /**
     * @brief Returns the id of the permission.
     */
    QString id() const;

    /**
     * @brief Sets the id of the permission.
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns a link back to this permission.
     */
    QUrl selfLink() const;

    /**
     * @brief Returns the name of this permission.
     */
    QString name() const;

    /**
     * @brief Returns the primary role for this user.
     */
    Permission::Role role() const;

    /**
     * @brief Sets the primary role for this user.
     */
    void setRole(Permission::Role role);

    /**
     * @brief Returns additional roles for this user. Only commenter is currently allowed.
     */
    QList<Role> additionalRoles() const;

    /**
     * @brief Sets additional roles for this user. Only commenter is currently allowed.
     *
     * @param additionalRoles
     */
    void setAdditionalRoles(const QList<Role> &additionalRoles);

    /**
     * @brief Returns the account type.
     */
    Permission::Type type() const;

    /**
     * @brief Sets the account type.
     *
     * @param type
     */
    void setType(Permission::Type type);

    /**
     * @brief Returns the authkey parameter required for this permission.
     */
    QString authKey() const;

    /**
     * @brief Returns whether the link is required for this permission.
     */
    bool withLink() const;

    /**
     * @brief Sets whether the link is required for this permission.
     *
     * @param withLink
     */
    void setWithLink(bool withLink);

    /**
     * @brief Returns a link to the profile photo, if available.
     */
    QUrl photoLink() const;

    /**
     * @brief Returns the email address or domain name for the entity.
     *
     * This is not populated in responses.
     * You can use the alias "me" as the value for this property to refer to the
     * current authorized user.
     */
    QString value() const;

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

    static PermissionPtr fromJSON(const QByteArray &jsonData);
    static PermissionsList fromJSONFeed(const QByteArray &jsonData);
    static QByteArray toJSON(const PermissionPtr &permission);

  private:
    class Private;
    Private *const d;
    friend class Private;
    friend class File::Private;
};

} /* namespace Drive */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_DRIVEPERMISSION_H
