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

#ifndef KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H
#define KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H

#include <libkgapi2/modifyjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{
namespace Blogger
{

class LIBKGAPI2_EXPORT CommentApproveJob : public KGAPI2::ModifyJob
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
                               QObject *parent = 0);
    explicit CommentApproveJob(const CommentPtr &comment,
                               ApprovalAction action,
                               const AccountPtr &account,
                               QObject *parent = 0);
    virtual ~CommentApproveJob();

  protected:
    void start();
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGPAI2

#endif // KGAPI2_BLOGGER_COMMENTAPPROVEJOB_H
