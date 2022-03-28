/*
    SPDX-FileCopyrightText: 2012 Andrius da Costa Ribas <andriusmao@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "file.h"
#include "kgapidrive_export.h"
#include "object.h"
#include "types.h"

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
class KGAPIDRIVE_EXPORT Permission : public KGAPI2::Object
{
public:
    enum Role {
        UndefinedRole = -1,
        OwnerRole = 0,
        ReaderRole = 1,
        WriterRole = 2,
        CommenterRole = 3,
        OrganizerRole = 4,
        FileOrganizerRole = 5,
    };

    enum Type { UndefinedType = -1, TypeUser = 0, TypeGroup = 1, TypeDomain = 2, TypeAnyone = 3 };

    /**
     * @brief Details of whether the permissions on this shared drive item are
     * inherited or directly on this item. This is an output-only field which
     * is present only for shared drive items.
     */
    class PermissionDetails
    {
    public:
        enum PermissionType {
            UndefinedType = -1,
            TypeFile = 0,
            TypeMember = 1,
        };

        PermissionDetails();
        PermissionDetails(const PermissionDetails &other);
        ~PermissionDetails();
        bool operator==(const PermissionDetails &other) const;
        bool operator!=(const PermissionDetails &other) const
        {
            return !operator==(other);
        }

        /**
         * @brief The permission type for this user.
         */
        PermissionDetails::PermissionType permissionType() const;

        /**
         * @brief The primary role for this user.
         */
        Permission::Role role() const;

        /**
         * @brief Additional roles for this user. Only commenter is currently possible,
         * though more may be supported in the future.
         */
        QList<Permission::Role> additionalRoles() const;

        /**
         * @brief The ID of the item from which this permission is inherited.
         * This is an output-only field and is only populated for members of
         * the shared drive.
         */
        QString inheritedFrom() const;

        /**
         * @brief Whether this permission is inherited. This field is always populated. This is an output-only field.
         */
        bool inherited() const;

    private:
        class Private;
        QScopedPointer<Private> const d;
        friend class Private;
        friend class Permission;
    };

    using PermissionDetailsPtr = QSharedPointer<PermissionDetails>;
    using PermissionDetailsList = QList<PermissionDetailsPtr>;

    explicit Permission();
    explicit Permission(const Permission &other);
    ~Permission() override;
    bool operator==(const Permission &other) const;
    bool operator!=(const Permission &other) const
    {
        return !operator==(other);
    }

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

    /**
     * @brief The email address of the user or group this permission refers to.
     * This is an output-only field which is present when the permission type is
     * user or group.
     */
    QString emailAddress() const;

    /**
     * @brief The domain name of the entity this permission refers to.
     * This is an output-only field which is present when the permission
     * type is user, group or domain.
     */
    QString domain() const;

    /**
     * @brief The time at which this permission will expire.
     */
    QDateTime expirationDate() const;

    /**
     * @brief Whether the account associated with this permission has been
     * deleted. This field only pertains to user and group permissions.
     */
    bool deleted() const;

    /**
     * @brief Details of whether the permissions on this shared drive
     * item are inherited or directly on this item.
     */
    Permission::PermissionDetailsList permissionDetails() const;

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
