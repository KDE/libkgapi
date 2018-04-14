/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
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
    QNetworkRequest createRequest(const QUrl &url);

    QString changeId;

    bool includeDeleted;
    bool includeSubscribed;
    int maxResults;
    qlonglong startChangeId;

  private:
    ChangeFetchJob *q;
};

ChangeFetchJob::Private::Private(ChangeFetchJob *parent):
    includeDeleted(true),
    includeSubscribed(true),
    maxResults(0),
    startChangeId(0),
    q(parent)
{
}

QNetworkRequest ChangeFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    return request;
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
        url.setQuery(query);
    } else {
        url = DriveService::fetchChangeUrl(d->changeId);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}


ObjectsList ChangeFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

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
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


