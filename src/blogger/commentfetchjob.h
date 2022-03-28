/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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

    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate)

    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate)

    Q_PROPERTY(bool fetchBodies READ fetchBodies WRITE setFetchBodies)

    Q_PROPERTY(uint maxResults READ maxResults WRITE setMaxResults)

public:
    explicit CommentFetchJob(const QString &blogId, const QString &postId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    explicit CommentFetchJob(const QString &blogId,
                             const QString &postId,
                             const QString &commentId,
                             const AccountPtr &account = AccountPtr(),
                             QObject *parent = nullptr);
    explicit CommentFetchJob(const QString &blogId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    ~CommentFetchJob() override;

    QDateTime endDate() const;
    void setEndDate(const QDateTime &endDate);

    QDateTime startDate() const;
    void setStartDate(const QDateTime &startDate);

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

    uint maxResults() const;
    void setMaxResults(uint maxResults);

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2
