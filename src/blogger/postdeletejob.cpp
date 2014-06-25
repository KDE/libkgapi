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

#include "postdeletejob.h"
#include "post.h"
#include "bloggerservice.h"

#include <libkgapi2/account.h>

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PostDeleteJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId);

    QString blogId;
    QString postId;
};

PostDeleteJob::Private::Private(const QString &blogId_,
                                const QString &postId_)
    : blogId(blogId_)
    , postId(postId_)
{
}

PostDeleteJob::PostDeleteJob(const QString &blogId,
                             const QString &postId,
                             const AccountPtr &account,
                             QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(blogId, postId))
{
}


PostDeleteJob::PostDeleteJob(const PostPtr &post,
                             const AccountPtr &account,
                             QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(post->blogId(), post->id()))
{
}

PostDeleteJob::~PostDeleteJob()
{
    delete d;
}

void PostDeleteJob::start()
{
    QNetworkRequest request(BloggerService::deletePostUrl(d->blogId, d->postId));
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    enqueueRequest(request);
}

void PostDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply);
    Q_UNUSED(rawData);

    emitFinished();
}
