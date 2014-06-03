/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
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
    explicit ChangeFetchJob(const AccountPtr &account, QObject *parent = 0);
    explicit ChangeFetchJob(const QString &changeId, const AccountPtr &account,
                            QObject *parent = 0);
    virtual ~ChangeFetchJob();

    bool includeSubscribed() const;
    void setIncludeSubscribed(bool includeSubscribed);

    bool includeDeleted() const;
    void setIncludeDeleted(bool includeDeleted);

    int maxResults() const;
    void setMaxResults(int maxResults);

    qlonglong startChangeId() const;
    void setStartChangeId(qlonglong startChangeId);

  protected:
    virtual void start();
    virtual KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVECHANGEFETCHJOB_H
