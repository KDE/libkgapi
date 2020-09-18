/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "commentdeletecontentjob.h"
#include "comment.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN CommentDeleteContentJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            const QString &commentId);

    const QString blogId;
    const QString postId;
    const QString commentId;
};

CommentDeleteContentJob::Private::Private(const QString &blogId_,
                                          const QString &postId_,
                                          const QString &commentId_)
    : blogId(blogId_)
    , postId(postId_)
    , commentId(commentId_)
{
}

CommentDeleteContentJob::CommentDeleteContentJob(const QString &blogId,
                                                 const QString &postId,
                                                 const QString &commentId,
                                                 const AccountPtr &account,
                                                 QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(blogId, postId, commentId))
{
}

CommentDeleteContentJob::CommentDeleteContentJob(const CommentPtr &comment,
                                                 const AccountPtr &account,
                                                 QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(comment->blogId(), comment->postId(), comment->id()))
{
}


CommentDeleteContentJob::~CommentDeleteContentJob()
{
    delete d;
}

void CommentDeleteContentJob::start()
{
    QNetworkRequest request(BloggerService::deleteCommentContentUrl(d->blogId, d->postId, d->commentId));
    enqueueRequest(request);
}

ObjectsList CommentDeleteContentJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    items << Comment::fromJSON(rawData);
    emitResult();
    return items;
}


