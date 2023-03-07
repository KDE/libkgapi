/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postpublishjob.h"
#include "bloggerservice.h"
#include "post.h"
#include "utils.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostPublishJob::Private
{
public:
    Private(const QString &blogId, const QString &postId, PostPublishJob::PublishAction action, const QDateTime &publishDate = QDateTime());

    const QString blogId;
    const QString postId;
    const PublishAction action;
    const QDateTime publishDate;

    ObjectPtr response;
};

PostPublishJob::Private::Private(const QString &blogId_, const QString &postId_, PostPublishJob::PublishAction action_, const QDateTime &publishDate_)
    : blogId(blogId_)
    , postId(postId_)
    , action(action_)
    , publishDate(publishDate_)
{
}

PostPublishJob::PostPublishJob(const PostPtr &post, PublishAction action, const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private(post->blogId(), post->id(), action))
{
}

PostPublishJob::PostPublishJob(const QString &blogId, const QString &postId, PublishAction action, const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private(blogId, postId, action))
{
}

PostPublishJob::PostPublishJob(const PostPtr &post, const QDateTime &publishDate, const AccountPtr &account, QObject *parent)
    : Job(account, parent)
    , d(new Private(post->blogId(), post->id(), Publish, publishDate))
{
}

PostPublishJob::PostPublishJob(const QString &blogId, const QString &postId, const QDateTime &publishDate, const AccountPtr &account, QObject *parent)
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
        QUrlQuery query(url);
        if (d->publishDate.isValid()) {
            query.addQueryItem(QStringLiteral("publishDate"), d->publishDate.toString(Qt::ISODate));
        }
        url.setQuery(query);
    } else {
        url = BloggerService::revertPostUrl(d->blogId, d->postId);
    }

    const QNetworkRequest request(url);
    enqueueRequest(request);
}

void PostPublishJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    Q_UNUSED(contentType)
    accessManager->post(request, data);
}

void PostPublishJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    d->response = Post::fromJSON(rawData);
    emitFinished();
}
