/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#ifndef KGAPI2_DRIVEPERMISSIONMODIFYJOB_H
#define KGAPI2_DRIVEPERMISSIONMODIFYJOB_H

#include "modifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT PermissionModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    explicit PermissionModifyJob(const QString &fileId,
                                 const PermissionPtr &permission,
                                 const AccountPtr &account, QObject *parent = nullptr);
    explicit PermissionModifyJob(const QString &fileId,
                                 const PermissionsList &permissions,
                                 const AccountPtr &account, QObject *parent = nullptr);
    ~PermissionModifyJob() override;

    /**
     * @brief Whether to remove the expiration date. (Default: false)
     */
    bool removeExpiration() const;

    /**
     * @brief Sets whether to remove the expiration date. (Default: false)
     */
    void setRemoveExpiration(bool removeExpiration);

    /**
     * @brief Whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED bool supportsAllDrives() const;

    /**
     * @brief Sets whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED void setSupportsAllDrives(bool supportsAllDrives);

    /**
     * @brief Whether changing a role to 'owner' downgrades the current owners
     * to writers. Does nothing if the specified role is not 'owner'.
     * (Default: false)
     */
    bool transferOwnership() const;

    /**
     * @brief Sets whether changing a role to 'owner' downgrades the current owners
     * to writers. Does nothing if the specified role is not 'owner'.
     * (Default: false)
     */
    void setTransferOwnership(bool transferOwnership);

    /**
     * @brief Issue the request as a domain administrator; if set to true,
     * then the requester will be granted access if the file ID parameter refers
     * to a shared drive and the requester is an administrator of the domain to
     * which the shared drive belongs. (Default: false)
     */
    bool useDomainAdminAccess() const;

    /**
     * @brief Sets to issue the request as a domain administrator; if set to true,
     * then the requester will be granted access if the file ID parameter refers
     * to a shared drive and the requester is an administrator of the domain to
     * which the shared drive belongs. (Default: false)
     */
    void setUseDomainAdminAccess(bool useDomainAdminAccess);

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEPERMISSIONMODIFYJOB_H
