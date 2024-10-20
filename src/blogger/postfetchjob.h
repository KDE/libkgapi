/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapiblogger_export.h"

#include <QStringList>

#include <QDateTime>

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PostFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    Q_PROPERTY(bool fetchBodies READ fetchBodies WRITE setFetchBodies)

    Q_PROPERTY(bool fetchImages READ fetchImages WRITE setFetchImages)

    Q_PROPERTY(uint maxResults READ maxResults WRITE setMaxResults)

    Q_PROPERTY(QStringList filterLabels READ filterLabels WRITE setFilterLabels)

    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate)

    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate)

    Q_PROPERTY(StatusFilters statusFilter READ statusFilter WRITE setStatusFilter)

    Q_FLAGS(StatusFilter)
public:
    enum StatusFilter {
        Draft = 1,
        Live = 2,
        Scheduled = 4,

        All = Draft | Live | Scheduled
    };
    Q_DECLARE_FLAGS(StatusFilters, StatusFilter)

    explicit PostFetchJob(const QString &blogId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    explicit PostFetchJob(const QString &blogId, const QString &postId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    ~PostFetchJob() override;

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

    bool fetchImages() const;
    void setFetchImages(bool fetchImages);

    uint maxResults() const;
    void setMaxResults(uint maxResults);

    QStringList filterLabels() const;
    void setFilterLabels(const QStringList &labels);

    QDateTime startDate() const;
    void setStartDate(const QDateTime &startDate);

    QDateTime endDate() const;
    void setEndDate(const QDateTime &endDate);

    void setStatusFilter(StatusFilters filter);
    StatusFilters statusFilter() const;

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};
}
}
