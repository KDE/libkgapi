/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
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

#ifndef KGAPI2_DRIVEDRIVESFETCHJOB_H
#define KGAPI2_DRIVEDRIVESFETCHJOB_H

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

#endif // KGAPI2_DRIVEDRIVESFETCHJOB_H
