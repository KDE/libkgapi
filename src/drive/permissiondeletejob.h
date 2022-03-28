/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT PermissionDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    explicit PermissionDeleteJob(const QString &fileId, const PermissionPtr &permission, const AccountPtr &account, QObject *parent = nullptr);
    explicit PermissionDeleteJob(const QString &fileId, const QString &permissionId, const AccountPtr &account, QObject *parent = nullptr);
    explicit PermissionDeleteJob(const QString &fileId, const PermissionsList &permissions, const AccountPtr &account, QObject *parent = nullptr);
    explicit PermissionDeleteJob(const QString &fileId, const QStringList &permissionsIds, const AccountPtr &account, QObject *parent = nullptr);
    ~PermissionDeleteJob() override;

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

private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
