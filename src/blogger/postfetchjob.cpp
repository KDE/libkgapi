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

#include "postfetchjob.h"
#include "post.h"
#include "bloggerservice.h"

#include <libkgapi2/account.h>
#include <libkgapi2/utils.h>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class PostFetchJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            PostFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);

    QString blogId;
    QString postId;

    bool fetchBodies;
    bool fetchImages;
    uint maxResults;
    QStringList filterLabels;
    KDateTime startDate;
    KDateTime endDate;
    StatusFilters statusFilter;

  private:
    PostFetchJob *q;
};

PostFetchJob::Private::Private(const QString &blogId_,
                               const QString &postId_,
                               PostFetchJob *parent)
    : blogId(blogId_)
    , postId(postId_)
    , fetchBodies(true)
    , fetchImages(true)
    , maxResults(0)
    , statusFilter(All)
    , q(parent)
{
}

QNetworkRequest PostFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    if (q->account()) {
        request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    }
    request.setUrl(url);

    return request;
}

PostFetchJob::PostFetchJob(const QString &blogId,
                           const AccountPtr &account,
                           QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, QString(), this))
{
}

PostFetchJob::PostFetchJob(const QString &blogId,
                           const QString &postId,
                           const AccountPtr &account,
                           QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, postId, this))
{
}

PostFetchJob::~PostFetchJob()
{
    delete d;
}

bool PostFetchJob::fetchBodies() const
{
    return d->fetchBodies;
}

void PostFetchJob::setFetchBodies(bool fetchBodies)
{
    d->fetchBodies = fetchBodies;
}

bool PostFetchJob::fetchImages() const
{
    return d->fetchImages;
}

void PostFetchJob::setFetchImages(bool fetchImages)
{
    d->fetchImages = fetchImages;
}

uint PostFetchJob::maxResults() const
{
    return d->maxResults;
}

void PostFetchJob::setMaxResults(uint maxResults)
{
    d->maxResults = maxResults;
}

QStringList PostFetchJob::filterLabels() const
{
    return d->filterLabels;
}

void PostFetchJob::setFilterLabels(const QStringList &labels)
{
    d->filterLabels = labels;
}

KDateTime PostFetchJob::startDate() const
{
    return d->startDate;
}

void PostFetchJob::setStartDate(const KDateTime &startDate)
{
    d->startDate = startDate;
}

KDateTime PostFetchJob::endDate() const
{
    return d->endDate;
}

void PostFetchJob::setEndDate(const KDateTime &endDate)
{
    d->endDate = endDate;
}

void PostFetchJob::setStatusFilter(PostFetchJob::StatusFilters filter)
{
    d->statusFilter = filter;
}

PostFetchJob::StatusFilters PostFetchJob::statusFilter() const
{
    return d->statusFilter;
}

void PostFetchJob::start()
{
    QUrl url = BloggerService::fetchPostUrl(d->blogId, d->postId);
    if (d->postId.isEmpty()) {
        if (d->startDate.isValid()) {
            url.addQueryItem(QLatin1String("startDate"), d->startDate.toString(KDateTime::RFC3339Date));
        }
        if (d->endDate.isValid()) {
            url.addQueryItem(QLatin1String("endDate"), d->endDate.toString(KDateTime::RFC3339Date));
        }
        if (d->maxResults > 0) {
            url.addQueryItem(QLatin1String("maxResults"), QString::number(d->maxResults));
        }
        if (!d->filterLabels.isEmpty())
            url.addQueryItem(QLatin1String("labels"), d->filterLabels.join(QLatin1String(",")));
        url.addQueryItem(QLatin1String("fetchBodies"), Utils::bool2Str(d->fetchBodies));
        url.addQueryItem(QLatin1String("fetchImages"), Utils::bool2Str(d->fetchImages));
    }
    if (account()) {
        url.addQueryItem(QLatin1String("view"), QLatin1String("ADMIN"));
    }
    if (d->statusFilter & Draft) {
        url.addQueryItem(QLatin1String("status"), QLatin1String("draft"));
    }
    if (d->statusFilter & Live) {
        url.addQueryItem(QLatin1String("status"), QLatin1String("live"));
    }
    if (d->statusFilter & Scheduled) {
        url.addQueryItem(QLatin1String("status"), QLatin1String("scheduled"));
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList PostFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->postId.isEmpty()) {
            items = Post::fromJSONFeed(rawData, feedData);
        } else {
            items << Post::fromJSON(rawData);
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

