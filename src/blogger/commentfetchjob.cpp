/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "commentfetchjob.h"
#include "bloggerservice.h"
#include "comment.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN CommentFetchJob::Private
{
public:
    Private(const QString &blogId, const QString &postId, const QString &commentId, CommentFetchJob *parent);
    ~Private();

    const QString blogId;
    const QString postId;
    const QString commentId;
    uint maxResults;
    QDateTime startDate;
    QDateTime endDate;
    bool fetchBodies;

private:
    CommentFetchJob *const q;
};

CommentFetchJob::Private::Private(const QString &blogId_, const QString &postId_, const QString &commentId_, CommentFetchJob *parent)
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

CommentFetchJob::CommentFetchJob(const QString &blogId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, QString(), QString(), this))
{
}

CommentFetchJob::CommentFetchJob(const QString &blogId, const QString &postId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, postId, QString(), this))
{
}

CommentFetchJob::CommentFetchJob(const QString &blogId, const QString &postId, const QString &commentId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, postId, commentId, this))
{
}

CommentFetchJob::~CommentFetchJob()
{
    delete d;
}

QDateTime CommentFetchJob::endDate() const
{
    return d->endDate;
}

void CommentFetchJob::setEndDate(const QDateTime &endDate)
{
    d->endDate = endDate;
}

QDateTime CommentFetchJob::startDate() const
{
    return d->startDate;
}

void CommentFetchJob::setStartDate(const QDateTime &startDate)
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
    QUrlQuery query(url);
    if (d->startDate.isValid()) {
        query.addQueryItem(QStringLiteral("startDate"), d->startDate.toString(Qt::ISODate));
    }
    if (d->endDate.isValid()) {
        query.addQueryItem(QStringLiteral("endDate"), d->endDate.toString(Qt::ISODate));
    }
    if (d->maxResults > 0) {
        query.addQueryItem(QStringLiteral("maxResults"), QString::number(d->maxResults));
    }
    query.addQueryItem(QStringLiteral("fetchBodies"), Utils::bool2Str(d->fetchBodies));
    if (account()) {
        query.addQueryItem(QStringLiteral("view"), QStringLiteral("ADMIN"));
    }
    url.setQuery(query);

    const QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList CommentFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->commentId.isEmpty()) {
            items = Comment::fromJSONFeed(rawData, feedData);
        } else {
            items << Comment::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request(feedData.nextPageUrl);
        enqueueRequest(request);
    } else {
        emitFinished();
    }

    return items;
}

#include "moc_commentfetchjob.cpp"
