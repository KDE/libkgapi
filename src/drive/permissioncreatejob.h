/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef KGAPI2_DRIVEPERMISSIONCREATEJOB_H
#define KGAPI2_DRIVEPERMISSIONCREATEJOB_H

#include "createjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT PermissionCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:
    explicit PermissionCreateJob(const QString &fileId,
                                 const PermissionPtr &permission,
                                 const AccountPtr &account, QObject *parent = nullptr);
    explicit PermissionCreateJob(const QString &fileId,
                                 const PermissionsList &permissions,
                                 const AccountPtr &account, QObject *parent = nullptr);
    ~PermissionCreateJob() override;

    /**
     * @brief The plain text custom message to include in notification emails.
     */
    QString emailMessage() const;

    /**
     * @brief Sets the plain text custom message to include in notification emails.
     */
    void setEmailMessage(const QString &emailMessage);

    /**
     * @brief Sets whether to send notification emails when sharing to users
     * or groups. This parameter is ignored and an email is sent if the role
     * is owner. (Default: true)
     */
    bool sendNotificationEmails() const;

    /**
     * @brief Whether to send notification emails when sharing to users or
     * groups. This parameter is ignored and an email is sent if the role
     * is owner. (Default: true)
     */
    void setSendNotificationEmails(bool sendNotificationEmails);

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
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData) override;

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEPERMISSIONCREATEJOB_H
