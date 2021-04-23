/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT CommentDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit CommentDeleteJob(const QString &blogId,
                              const QString &postId,
                              const QString &commentId,
                              const AccountPtr &account,
                              QObject *parent = nullptr);
    explicit CommentDeleteJob(const CommentPtr &comment,
                              const AccountPtr &account,
                              QObject *parent = nullptr);
    ~CommentDeleteJob() override;

  protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

