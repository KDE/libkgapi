/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "commentdeletejob.h"
#include "bloggerservice.h"
#include "comment.h"
#include "utils.h"
#include "account.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN CommentDeleteJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            const QString &commentId);

    QString blogId;
    QString postId;
    QString commentId;

};

CommentDeleteJob::Private::Private(const QString &blogId_,
                                   const QString &postId_,
                                   const QString &commentId_)
    : blogId(blogId_)
    , postId(postId_)
    , commentId(commentId_)
{
}

CommentDeleteJob::CommentDeleteJob(const CommentPtr &comment,
                                   const AccountPtr &account,
                                   QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(comment->blogId(), comment->postId(), comment->id()))
{

}

CommentDeleteJob::CommentDeleteJob(const QString &blogId,
                                   const QString &postId,
                                   const QString &commentId,
                                   const AccountPtr &account,
                                   QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(blogId, postId, commentId))
{
}

CommentDeleteJob::~CommentDeleteJob()
{
    delete d;
}


void CommentDeleteJob::start()
{
    QNetworkRequest request(BloggerService::deleteCommentUrl(d->blogId, d->postId, d->commentId));

    enqueueRequest(request);
}

void CommentDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    emitFinished();
}



