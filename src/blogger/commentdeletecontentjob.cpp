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

#include "commentdeletecontentjob.h"
#include "comment.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class CommentDeleteContentJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            const QString &commentId);

    QString blogId;
    QString postId;
    QString commentId;
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
        emitFinished();
        return items;
    }

    items << Comment::fromJSON(rawData);
    emitFinished();
    return items;
}


