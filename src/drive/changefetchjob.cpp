/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "changefetchjob.h"
#include "account.h"
#include "change.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChangeFetchJob::Private
{
  public:
    Private(ChangeFetchJob *parent);

    QString changeId;

    bool includeDeleted = true;
    bool includeSubscribed = true;
    int maxResults = 0;
    qlonglong startChangeId = 0;
    bool includeItemsFromAllDrives = true;
    bool supportsAllDrives = true;

  private:
    ChangeFetchJob *const q;
};

ChangeFetchJob::Private::Private(ChangeFetchJob *parent):
    q(parent)
{
}

ChangeFetchJob::ChangeFetchJob(const QString &changeId,
                               const AccountPtr &account,
                               QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->changeId = changeId;
}

ChangeFetchJob::ChangeFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

ChangeFetchJob::~ChangeFetchJob()
{
    delete d;
}

void ChangeFetchJob::setIncludeDeleted(bool includeDeleted)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify includeDeleted property when job is running";
        return;
    }

    d->includeDeleted = includeDeleted;
}

bool ChangeFetchJob::includeDeleted() const
{
    return d->includeDeleted;
}

void ChangeFetchJob::setIncludeSubscribed(bool includeSubscribed)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify includeSubscribed property when job is running";
        return;
    }

    d->includeSubscribed = includeSubscribed;
}

bool ChangeFetchJob::includeSubscribed() const
{
    return d->includeSubscribed;
}

void ChangeFetchJob::setMaxResults(int maxResults)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property when job is running";
        return;
    }

    d->maxResults = maxResults;
}

int ChangeFetchJob::maxResults() const
{
    return d->maxResults;
}

void ChangeFetchJob::setStartChangeId(qlonglong startChangeId)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify startChangeId property when job is running";
    }

    d->startChangeId = startChangeId;
}

qlonglong ChangeFetchJob::startChangeId() const
{
    return d->startChangeId;
}

bool ChangeFetchJob::includeItemsFromAllDrives() const
{
    return d->includeItemsFromAllDrives;
}

void ChangeFetchJob::setIncludeItemsFromAllDrives(bool includeItemsFromAllDrives)
{
    d->includeItemsFromAllDrives = includeItemsFromAllDrives;
}

bool ChangeFetchJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void ChangeFetchJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

void ChangeFetchJob::start()
{
    QUrl url;
    if (d->changeId.isEmpty()) {
        url = DriveService::fetchChangesUrl();
        QUrlQuery query(url);
        query.addQueryItem(QStringLiteral("includeDeleted"), Utils::bool2Str(d->includeDeleted));
        query.addQueryItem(QStringLiteral("includeSubscribed"), Utils::bool2Str(d->includeSubscribed));
        if (d->maxResults > 0) {
            query.addQueryItem(QStringLiteral("maxResults"), QString::number(d->maxResults));
        }
        if (d->startChangeId > 0) {
            query.addQueryItem(QStringLiteral("startChangeId"), QString::number(d->startChangeId));
        }
        query.addQueryItem(QStringLiteral("includeItemsFromAllDrives"), Utils::bool2Str(d->includeItemsFromAllDrives));
        url.setQuery(query);
    } else {
        url = DriveService::fetchChangeUrl(d->changeId);
    }

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(d->supportsAllDrives));
    url.setQuery(query);

    QNetworkRequest request(url);
    enqueueRequest(request);
}


ObjectsList ChangeFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->changeId.isEmpty()) {
            items << Change::fromJSONFeed(rawData, feedData);
        } else {
            items << Change::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        QNetworkRequest request(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


