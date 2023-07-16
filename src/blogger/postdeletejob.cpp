/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postdeletejob.h"
#include "bloggerservice.h"
#include "post.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostDeleteJob::Private
{
public:
    Private(const QString &blogId, const QString &postId);

    const QString blogId;
    const QString postId;
};

PostDeleteJob::Private::Private(const QString &blogId_, const QString &postId_)
    : blogId(blogId_)
    , postId(postId_)
{
}

PostDeleteJob::PostDeleteJob(const QString &blogId, const QString &postId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(blogId, postId))
{
}

PostDeleteJob::PostDeleteJob(const PostPtr &post, const AccountPtr &account, QObject *parent)
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
    const QUrl url = BloggerService::deletePostUrl(d->blogId, d->postId);
    QNetworkRequest request(url);
    enqueueRequest(request);
}

void PostDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    emitFinished();
}

#include "moc_postdeletejob.cpp"
