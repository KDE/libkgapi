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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KGAPI2_DRIVECHANGEFETCHJOB_H
#define KGAPI2_DRIVECHANGEFETCHJOB_H

#include "fetchjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT ChangeFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Whether to include deleted items.
     *
     * By default deletes items are included.
     *
     * This property does not have any effect when fetching a specific change and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(bool includeDeleted
               READ includeDeleted
               WRITE setIncludeDeleted)

    /**
     * Whether to include shared files and public files the user has opened.
     * When set to false, the list will include owned files plus any shared or
     * public files the user has explictly added to a folder in Drive.
     *
     * Default is to include subscribed files.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(bool includeSubscribed
               READ includeSubscribed
               WRITE setIncludeSubscribed)

    /**
     * Maximum number of changes to return.
     *
     * Default value is 0, i.e. no limit.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(int maxResults
               READ maxResults
               WRITE setMaxResults)

    /**
     * Change ID to start listing changes from.
     *
     * Default value is 0, i.e. all changes.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     */
    Q_PROPERTY(qlonglong startChangeId
               READ startChangeId
               WRITE setStartChangeId)

  public:
    explicit ChangeFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    explicit ChangeFetchJob(const QString &changeId, const AccountPtr &account,
                            QObject *parent = nullptr);
    ~ChangeFetchJob() override;

    bool includeSubscribed() const;
    void setIncludeSubscribed(bool includeSubscribed);

    bool includeDeleted() const;
    void setIncludeDeleted(bool includeDeleted);

    int maxResults() const;
    void setMaxResults(int maxResults);

    qlonglong startChangeId() const;
    void setStartChangeId(qlonglong startChangeId);

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVECHANGEFETCHJOB_H
