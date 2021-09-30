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

#include "teamdrivesearchquery.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_DEPRECATED_EXPORT TeamdriveFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Maximum number of teamdrives to return.
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
     * Team Drives of the domain in which the requester is an administrator
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
    TeamdriveFetchJob(const TeamdriveSearchQuery &query, const AccountPtr &account, QObject *parent = nullptr);
    explicit TeamdriveFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    TeamdriveFetchJob(const QString &teamdriveId, const AccountPtr &account,
                            QObject *parent = nullptr);
    ~TeamdriveFetchJob() override;

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

