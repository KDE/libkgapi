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

#include "commentfetchjob.h"
#include "comment.h"
#include "bloggerservice.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <libkgapi2/types.h>
#include <libkgapi2/utils.h>
#include <libkgapi2/account.h>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class CommentFetchJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            const QString &commentId,
            CommentFetchJob *parent);
    ~Private();

    QNetworkRequest createRequest(const QUrl &url);

    QString blogId;
    QString postId;
    QString commentId;
    uint maxResults;
    KDateTime startDate;
    KDateTime endDate;
    bool fetchBodies;

  private:
    CommentFetchJob *q;
};

CommentFetchJob::Private::Private(const QString &blogId_,
                                  const QString &postId_,
                                  const QString &commentId_,
                                  CommentFetchJob *parent)
    : blogId(blogId_)
    , postId(postId_)
    , commentId(commentId_)
    , maxResults(0)
    , fetchBodies(true)
    , q(parent)
{
}

CommentFetchJob::Private::~Private()
{
}

QNetworkRequest CommentFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    if (q->account()) {
        request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    }
    request.setUrl(url);

    return request;
}


CommentFetchJob::CommentFetchJob(const QString &blogId,
                                 const AccountPtr &account,
                                 QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, QString(), QString(), this))
{

}

CommentFetchJob::CommentFetchJob(const QString &blogId,
                                 const QString &postId,
                                 const AccountPtr &account,
                                 QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, postId, QString(), this))
{

}

CommentFetchJob::CommentFetchJob(const QString &blogId,
                                 const QString &postId,
                                 const QString &commentId,
                                 const AccountPtr &account,
                                 QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, postId, commentId, this))
{
}

CommentFetchJob::~CommentFetchJob()
{
    delete d;
}

KDateTime CommentFetchJob::endDate() const
{
    return d->endDate;
}

void CommentFetchJob::setEndDate(const KDateTime &endDate)
{
    d->endDate = endDate;
}

KDateTime CommentFetchJob::startDate() const
{
    return d->startDate;
}


void CommentFetchJob::setStartDate(const KDateTime &startDate)
{
    d->startDate = startDate;
}

uint CommentFetchJob::maxResults() const
{
    return d->maxResults;
}

void CommentFetchJob::setMaxResults(uint maxResults)
{
    d->maxResults = maxResults;
}

bool CommentFetchJob::fetchBodies() const
{
    return d->fetchBodies;
}

void CommentFetchJob::setFetchBodies(bool fetchBodies)
{
    d->fetchBodies = fetchBodies;
}

void CommentFetchJob::start()
{
    QUrl url = BloggerService::fetchCommentsUrl(d->blogId, d->postId, d->commentId);

    if (d->startDate.isValid()) {
        url.addQueryItem(QLatin1String("startDate"), d->startDate.toString(KDateTime::RFC3339Date));
    }
    if (d->endDate.isValid()) {
        url.addQueryItem(QLatin1String("endDate"), d->endDate.toString(KDateTime::RFC3339Date));
    }
    if (d->maxResults > 0) {
        url.addQueryItem(QLatin1String("maxResults"), QString::number(d->maxResults));
    }
    url.addQueryItem(QLatin1String("fetchBodies"), Utils::bool2Str(d->fetchBodies));
    if (account()) {
        url.addQueryItem(QLatin1String("view"), QLatin1String("ADMIN"));
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList CommentFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->commentId.isEmpty()) {
            items =  Comment::fromJSONFeed(rawData, feedData);
        } else {
            items << Comment::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    } else {
        emitFinished();
    }

    return items;
}

#include "commentfetchjob.moc"
