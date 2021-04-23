/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapidrive_export.h"

#include "drivessearchquery.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT DrivesFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Maximum number of shared drives to return. Acceptable
     * values are 1 to 100, inclusive
     *
     * Default value if missing is 10.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(int maxResults
               READ maxResults
               WRITE setMaxResults)

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
    DrivesFetchJob(const DrivesSearchQuery &query, const AccountPtr &account, QObject *parent = nullptr);
    DrivesFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    DrivesFetchJob(const QString &drivesId, const AccountPtr &account,
                            QObject *parent = nullptr);
    ~DrivesFetchJob() override;

    int maxResults() const;
    void setMaxResults(int maxResults);

    void setUseDomainAdminAccess(bool useDomainAdminAccess);
    bool useDomainAdminAccess() const;

    void setFields(const QStringList &fields);
    QStringList fields() const;

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;

    void applyRequestParameters(QUrl &url);
};

} // namespace Drive

} // namespace KGAPI2

