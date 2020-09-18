/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postfetchjob.h"
#include "post.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostFetchJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &postId,
            PostFetchJob *parent);

    QString blogId;
    QString postId;

    bool fetchBodies = true;
    bool fetchImages = true;
    uint maxResults = 0;
    QStringList filterLabels;
    QDateTime startDate;
    QDateTime endDate;
    StatusFilters statusFilter = All;

  private:
    PostFetchJob *const q;
};

PostFetchJob::Private::Private(const QString &blogId_,
                               const QString &postId_,
                               PostFetchJob *parent)
    : blogId(blogId_)
    , postId(postId_)
    , q(parent)
{
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

QDateTime PostFetchJob::startDate() const
{
    return d->startDate;
}

void PostFetchJob::setStartDate(const QDateTime &startDate)
{
    d->startDate = startDate;
}

QDateTime PostFetchJob::endDate() const
{
    return d->endDate;
}

void PostFetchJob::setEndDate(const QDateTime &endDate)
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
    QUrlQuery query(url);
    if (d->postId.isEmpty()) {
        if (d->startDate.isValid()) {
            query.addQueryItem(QStringLiteral("startDate"), d->startDate.toString(Qt::ISODate));
        }
        if (d->endDate.isValid()) {
            query.addQueryItem(QStringLiteral("endDate"), d->endDate.toString(Qt::ISODate));
        }
        if (d->maxResults > 0) {
            query.addQueryItem(QStringLiteral("maxResults"), QString::number(d->maxResults));
        }
        if (!d->filterLabels.isEmpty())
            query.addQueryItem(QStringLiteral("labels"), d->filterLabels.join(QLatin1Char(',')));
        query.addQueryItem(QStringLiteral("fetchBodies"), Utils::bool2Str(d->fetchBodies));
        query.addQueryItem(QStringLiteral("fetchImages"), Utils::bool2Str(d->fetchImages));
    }
    if (account()) {
        query.addQueryItem(QStringLiteral("view"), QStringLiteral("ADMIN"));
    }
    if (d->statusFilter & Draft) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("draft"));
    }
    if (d->statusFilter & Live) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("live"));
    }
    if (d->statusFilter & Scheduled) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("scheduled"));
    }
    url.setQuery(query);
    const QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList PostFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
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
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request(feedData.nextPageUrl);
        enqueueRequest(request);
    } else {
        emitResult();
    }

    return items;
}

