/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "postsearchjob.h"
#include "post.h"
#include "bloggerservice.h"
#include "account.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PostSearchJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &query,
            PostSearchJob *parent);

    const QString blogId;
    const QString query;
    bool fetchBodies = true;

  private:
    PostSearchJob *const q;
};

PostSearchJob::Private::Private(const QString &blogId_,
                                const QString &query_,
                                PostSearchJob *parent)
    : blogId(blogId_)
    , query(query_)
    , fetchBodies(true)
    , q(parent)
{
}

PostSearchJob::PostSearchJob(const QString &blogId,
                             const QString &query,
                             const AccountPtr &account,
                             QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, query, this))
{
}

PostSearchJob::~PostSearchJob()
{
    delete d;
}

bool PostSearchJob::fetchBodies() const
{
    return d->fetchBodies;
}

void PostSearchJob::setFetchBodies(bool fetchBodies)
{
    d->fetchBodies = fetchBodies;
}

void PostSearchJob::start()
{
    QUrl url = BloggerService::searchPostUrl(d->blogId);
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("q"), d->query);
    query.addQueryItem(QStringLiteral("fetchBodies"), Utils::bool2Str(d->fetchBodies));
    url.setQuery(query);
    const QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList PostSearchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        items = Post::fromJSONFeed(rawData, feedData);
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

