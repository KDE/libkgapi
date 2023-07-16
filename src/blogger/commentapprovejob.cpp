/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "commentapprovejob.h"
#include "account.h"
#include "bloggerservice.h"
#include "comment.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN CommentApproveJob::Private
{
public:
    Private(const QString &blogId, const QString &postId, const QString &commentId, ApprovalAction action);

    const QString blogId;
    const QString postId;
    const QString commentId;
    const ApprovalAction action;
};

CommentApproveJob::Private::Private(const QString &blogId_, const QString &postId_, const QString &commentId_, CommentApproveJob::ApprovalAction action_)
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

CommentApproveJob::CommentApproveJob(const CommentPtr &comment, CommentApproveJob::ApprovalAction action, const AccountPtr &account, QObject *parent)
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
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << Comment::fromJSON(rawData);
    emitFinished();
    return items;
}

#include "moc_commentapprovejob.cpp"
