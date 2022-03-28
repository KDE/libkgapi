/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "blogfetchjob.h"
#include "account.h"
#include "blog.h"
#include "bloggerservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN BlogFetchJob::Private
{
public:
    Private(const QString &id, FetchBy fetchBy);
    ~Private();

    const QString id;
    const FetchBy fetchBy;
};

BlogFetchJob::Private::Private(const QString &id_, FetchBy fetchBy_)
    : id(id_)
    , fetchBy(fetchBy_)
{
}

BlogFetchJob::Private::~Private()
{
}

BlogFetchJob::BlogFetchJob(const QString &id, FetchBy fetchBy, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(id, fetchBy))
{
}

BlogFetchJob::~BlogFetchJob()
{
    delete d;
}

void BlogFetchJob::start()
{
    QNetworkRequest request;
    switch (d->fetchBy) {
    case FetchByBlogId:
        request.setUrl(BloggerService::fetchBlogByBlogIdUrl(d->id));
        break;
    case FetchByBlogUrl:
        request.setUrl(BloggerService::fetchBlogByBlogUrlUrl(d->id));
        break;
    case FetchByUserId:
        request.setUrl(BloggerService::fetchBlogsByUserIdUrl(d->id));
        break;
    }

    enqueueRequest(request);
}

ObjectsList BlogFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->fetchBy == FetchByUserId) {
            items << Blog::fromJSONFeed(rawData);
        } else {
            items << Blog::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}
