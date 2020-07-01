/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#ifndef KGAPI2_DRIVEDRIVESMODIFYJOB_H
#define KGAPI2_DRIVEDRIVESMODIFYJOB_H

#include "modifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT DrivesModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

    /**
     * Issue the request as a domain administrator; if set to true, then all
     * shared Drives of the domain in which the requester is an administrator
     * are returned.
     *
     * Default value if missing is false.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(bool useDomainAdminAccess
               READ useDomainAdminAccess
               WRITE setUseDomainAdminAccess)

  public:
    explicit DrivesModifyJob(const DrivesPtr &drives,
                                 const AccountPtr &account, QObject *parent = nullptr);
    explicit DrivesModifyJob(const DrivesList &drives,
                                 const AccountPtr &account, QObject *parent = nullptr);
    ~DrivesModifyJob() override;

    void setUseDomainAdminAccess(bool useDomainAdminAccess);
    bool useDomainAdminAccess() const;

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

#endif // KGAPI2_DRIVEDRIVESMODIFYJOB_H
