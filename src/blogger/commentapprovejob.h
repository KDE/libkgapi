/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H
#define KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H

#include "modifyjob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT CommentApproveJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    enum ApprovalAction {
        Approve,
        MarkAsSpam
    };
    explicit CommentApproveJob(const QString &blogId,
                               const QString &postId,
                               const QString &commentId,
                               ApprovalAction action,
                               const AccountPtr &account,
                               QObject *parent = nullptr);
    explicit CommentApproveJob(const CommentPtr &comment,
                               ApprovalAction action,
                               const AccountPtr &account,
                               QObject *parent = nullptr);
    ~CommentApproveJob() override;

  protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

#endif // KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H
