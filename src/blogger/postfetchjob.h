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

#ifndef KGAPI2_BLOGGER_POSTFETCHJOB_H
#define KGAPI2_BLOGGER_POSTFETCHJOB_H

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QStringList>

#include <KDateTime>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT PostFetchJob : public  KGAPI2::FetchJob
{
    Q_OBJECT

    Q_PROPERTY(bool fetchBodies
               READ fetchBodies
               WRITE setFetchBodies)

    Q_PROPERTY(bool fetchImages
               READ fetchImages
               WRITE setFetchImages)

    Q_PROPERTY(uint maxResults
               READ maxResults
               WRITE setMaxResults)

    Q_PROPERTY(QStringList filterLabels
               READ filterLabels
               WRITE setFilterLabels)

    Q_PROPERTY(KDateTime startDate
               READ startDate
               WRITE setStartDate)

    Q_PROPERTY(KDateTime endDate
               READ endDate
               WRITE setEndDate)

    Q_PROPERTY(StatusFilter statusFilter
               READ statusFilter
               WRITE setStatusFilter)

    Q_FLAGS(StatusFilter)
  public:
    enum StatusFilter {
        Draft = 1,
        Live = 2,
        Scheduled = 4,

        All = Draft | Live | Scheduled
    };
    Q_DECLARE_FLAGS(StatusFilters, StatusFilter)

    explicit PostFetchJob(const QString &blogId,
                          const AccountPtr &account = AccountPtr(),
                          QObject *parent = 0);
    explicit PostFetchJob(const QString &blogId,
                          const QString &postId,
                          const AccountPtr &account = AccountPtr(),
                          QObject *parent = 0);
    virtual ~PostFetchJob();

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

    bool fetchImages() const;
    void setFetchImages(bool fetchImages);

    uint maxResults() const;
    void setMaxResults(uint maxResults);

    QStringList filterLabels() const;
    void setFilterLabels(const QStringList &labels);

    KDateTime startDate() const;
    void setStartDate(const KDateTime &startDate);

    KDateTime endDate() const;
    void setEndDate(const KDateTime &endDate);

    void setStatusFilter(StatusFilters filter);
    StatusFilters statusFilter() const;

  protected:
    void start();
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData);

private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_POSTFETCHJOB_H
