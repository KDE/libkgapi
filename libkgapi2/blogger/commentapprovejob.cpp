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

#include "commentapprovejob.h"
#include "comment.h"
#include "bloggerservice.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class CommentApproveJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            const QString &commentId,
            ApprovalAction action);

    QString blogId;
    QString postId;
    QString commentId;
    ApprovalAction action;
};

CommentApproveJob::Private::Private(const QString &blogId_,
                                    const QString &postId_,
                                    const QString &commentId_,
                                    CommentApproveJob::ApprovalAction action_)
    : blogId(blogId_)
    , postId(postId_)
    , commentId(commentId_)
    , action(action_)
{
}

CommentApproveJob::CommentApproveJob(const QString &blogId,
                                     const QString &postId,
                                     const QString &commentId,
                                     CommentApproveJob::ApprovalAction action,
                                     const AccountPtr &account,
                                     QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(blogId, postId, commentId, action))
{
}

CommentApproveJob::CommentApproveJob(const CommentPtr &comment,
                                     CommentApproveJob::ApprovalAction action,
                                     const AccountPtr &account,
                                     QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(comment->blogId(), comment->postId(), comment->id(), action))
{
}


CommentApproveJob::~CommentApproveJob()
{
    delete d;
}

void CommentApproveJob::start()
{
    QNetworkRequest request;
    if (d->action == Blogger::CommentApproveJob::Approve) {
        request.setUrl(BloggerService::approveCommentUrl(d->blogId, d->postId, d->commentId));
    } else {
        request.setUrl(BloggerService::markCommentAsSpamUrl(d->blogId, d->postId, d->commentId));
    }

    enqueueRequest(request);
}

ObjectsList CommentApproveJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << Comment::fromJSON(rawData);
    emitFinished();
    return items;
}

#include "commentapprovejob.moc"
