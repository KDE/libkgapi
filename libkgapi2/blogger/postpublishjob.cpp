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

#include "postpublishjob.h"
#include "post.h"
#include "bloggerservice.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PostPublishJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            PostPublishJob::PublishAction action,
            const KDateTime &publishDate = KDateTime());

    QString blogId;
    QString postId;
    PublishAction action;
    KDateTime publishDate;

    ObjectPtr response;
};

PostPublishJob::Private::Private(const QString &blogId_,
                                 const QString &postId_,
                                 PostPublishJob::PublishAction action_,
                                 const KDateTime &publishDate_)
    : blogId(blogId_)
    , postId(postId_)
    , action(action_)
    , publishDate(publishDate_)
{
}

PostPublishJob::PostPublishJob(const PostPtr &post,
                               PublishAction action,
                               const AccountPtr &account,
                               QObject *parent)
    : Job(account, parent)
    , d(new Private(post->blogId(), post->id(), action))
{
}

PostPublishJob::PostPublishJob(const QString &blogId,
                               const QString &postId,
                               PublishAction action,
                               const AccountPtr &account,
                               QObject *parent)
    : Job(account, parent)
    , d(new Private(blogId, postId, action))
{
}

PostPublishJob::PostPublishJob(const PostPtr &post,
                               const KDateTime &publishDate,
                               const AccountPtr &account,
                               QObject *parent)
    : Job(account, parent)
    , d(new Private(post->blogId(), post->id(), Publish, publishDate))
{
}

PostPublishJob::PostPublishJob(const QString &blogId,
                               const QString &postId,
                               const KDateTime &publishDate,
                               const AccountPtr &account,
                               QObject *parent)
    : Job(account, parent)
    , d(new Private(blogId, postId, Publish, publishDate))
{
}


PostPublishJob::~PostPublishJob()
{
    delete d;
}

ObjectPtr PostPublishJob::item() const
{
    return d->response;
}

void PostPublishJob::start()
{
    QUrl url;
    if (d->action == Blogger::PostPublishJob::Publish) {
        url = BloggerService::publishPostUrl(d->blogId, d->postId);
        if (d->publishDate.isValid()) {
            url.addQueryItem(QLatin1String("publishDate"), d->publishDate.toString(KDateTime::RFC3339Date));
        }
    } else {
        url = BloggerService::revertPostUrl(d->blogId, d->postId);
    }

    const QNetworkRequest request(url);
    enqueueRequest(request);
}

void PostPublishJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                     const QNetworkRequest &request,
                                     const QByteArray &data,
                                     const QString &contentType)
{
    Q_UNUSED(contentType);
    accessManager->post(request, data);
}

void PostPublishJob::handleReply(const QNetworkReply *reply,
                                 const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    d->response = Post::fromJSON(rawData);
    emitFinished();
}


