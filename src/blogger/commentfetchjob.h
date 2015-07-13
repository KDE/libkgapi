/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KGAPI2_BLOGGER_COMMENTFETCHJOB_H
#define KGAPI2_BLOGGER_COMMENTFETCHJOB_H

#include "fetchjob.h"
#include "kgapiblogger_export.h"

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT CommentFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    Q_PROPERTY(QDateTime endDate
               READ endDate
               WRITE setEndDate)

    Q_PROPERTY(QDateTime startDate
               READ startDate
               WRITE setStartDate)

    Q_PROPERTY(bool fetchBodies
               READ fetchBodies
               WRITE setFetchBodies)

    Q_PROPERTY(uint maxResults
               READ maxResults
               WRITE setMaxResults)

  public:
    explicit CommentFetchJob(const QString &blogId, const QString &postId,
                             const AccountPtr &account = AccountPtr(),
                             QObject *parent = 0);
    explicit CommentFetchJob(const QString &blogId, const QString &postId,
                             const QString &commentId,
                             const AccountPtr &account = AccountPtr(),
                             QObject *parent = 0);
    explicit CommentFetchJob(const QString &blogId,
                             const AccountPtr &account = AccountPtr(),
                             QObject *parent = 0);
    virtual ~CommentFetchJob();

    QDateTime endDate() const;
    void setEndDate(const QDateTime &endDate);

    QDateTime startDate() const;
    void setStartDate(const QDateTime &startDate);

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

    uint maxResults() const;
    void setMaxResults(uint maxResults);

  protected:
    void start() Q_DECL_OVERRIDE;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

#endif // KGAPI2_BLOGGER_COMMENTFETCHJOB_H
