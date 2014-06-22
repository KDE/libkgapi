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

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDateTime>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT CommentFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    Q_PROPERTY(KDateTime endDate
               READ endDate
               WRITE setEndDate)

    Q_PROPERTY(KDateTime startDate
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

    KDateTime endDate() const;
    void setEndDate(const KDateTime &endDate);

    KDateTime startDate() const;
    void setStartDate(const KDateTime &startDate);

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

    uint maxResults() const;
    void setMaxResults(uint maxResults);

  protected:
    void start();
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

#endif // KGAPI2_BLOGGER_COMMENTFETCHJOB_H
