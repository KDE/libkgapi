/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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

    QNetworkRequest createRequest(const QUrl &url);

    QString blogId;
    QString query;
    bool fetchBodies;

  private:
    PostSearchJob *q;
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

QNetworkRequest PostSearchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    if (q->account()) {
        request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    }
    request.setUrl(url);

    return request;
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
    const QNetworkRequest request = d->createRequest(url);
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
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    } else {
        emitFinished();
    }

    return items;
}

