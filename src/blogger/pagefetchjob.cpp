/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "pagefetchjob.h"
#include "bloggerservice.h"
#include "page.h"
#include "utils.h"
#include "account.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Blogger;

class Q_DECL_HIDDEN PageFetchJob::Private
{
  public:
    Private(const QString &blogId,
            const QString &pageId);

    QString blogId;
    QString pageId;

    bool fetchContent;
    StatusFilters statusFilter;
};

PageFetchJob::Private::Private(const QString &blogId_,
                               const QString &pageId_)
    : blogId(blogId_)
    , pageId(pageId_)
    , fetchContent(true)
    , statusFilter(All)
{
}

PageFetchJob::PageFetchJob(const QString &blogId,
                           const AccountPtr &account,
                           QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, QString()))
{
}

PageFetchJob::PageFetchJob(const QString &blogId,
                           const QString &pageId,
                           const AccountPtr &account,
                           QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(blogId, pageId))
{
}

PageFetchJob::~PageFetchJob()
{
    delete d;
}

bool PageFetchJob::fetchContent() const
{
    return d->fetchContent;
}

void PageFetchJob::setFetchContent(bool fetchContent)
{
    d->fetchContent = fetchContent;
}

PageFetchJob::StatusFilters PageFetchJob::statusFilter() const
{
    return d->statusFilter;
}

void PageFetchJob::setStatusFilter(StatusFilters status)
{
    d->statusFilter = status;
}

void PageFetchJob::start()
{
    QUrl url = BloggerService::fetchPageUrl(d->blogId, d->pageId);
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("fetchBodies"), Utils::bool2Str(d->fetchContent));
    if (d->statusFilter & Draft) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("draft"));
    }
    if (d->statusFilter & Imported) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("imported"));
    }
    if (d->statusFilter & Live) {
        query.addQueryItem(QStringLiteral("status"), QStringLiteral("live"));
    }
    if (account()) {
        query.addQueryItem(QStringLiteral("view"), QStringLiteral("ADMIN"));
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList PageFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->pageId.isEmpty()) {
            items =  Page::fromJSONFeed(rawData);
        } else {
            items << Page::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    emitFinished();
    return items;
}


