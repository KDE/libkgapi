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

#include "locationfetchhistoryjob.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"
#include "latitudeservice.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN LocationFetchHistoryJob::Private
{
  public:
    Private(LocationFetchHistoryJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    Latitude::Granularity granularity;
    int maxResults;
    qlonglong minTimestamp;
    qlonglong maxTimestamp;

  private:
    LocationFetchHistoryJob * const q;
};

LocationFetchHistoryJob::Private::Private(LocationFetchHistoryJob *parent):
    granularity(Latitude::City),
    maxResults(0),
    minTimestamp(0),
    maxTimestamp(0),
    q(parent)
{
}

QNetworkRequest LocationFetchHistoryJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());
    request.setUrl(url);

    return request;
}


LocationFetchHistoryJob::LocationFetchHistoryJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

LocationFetchHistoryJob::~LocationFetchHistoryJob()
{
    delete d;
}

int LocationFetchHistoryJob::maxResults() const
{
    return d->maxResults;
}

void LocationFetchHistoryJob::setMaxResults(int results)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property while job is running";
    }
    d->maxResults = results;
}

Latitude::Granularity LocationFetchHistoryJob::granularity() const
{
    return d->granularity;
}

void LocationFetchHistoryJob::setGranularity(Latitude::Granularity granularity)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property while job is running";
    }
    d->granularity = granularity;
}

qlonglong LocationFetchHistoryJob::minTimestamp() const
{
    return d->minTimestamp;
}

void LocationFetchHistoryJob::setMinTimestamp(qlonglong minTimestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property while job is running";
    }
    d->minTimestamp = minTimestamp;
}

qlonglong LocationFetchHistoryJob::maxTimestamp() const
{
    return d->maxTimestamp;
}

void LocationFetchHistoryJob::setMaxTimestamp(qlonglong maxTimestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property while job is running";
    }
    d->maxTimestamp = maxTimestamp;
}

void LocationFetchHistoryJob::start()
{
    const QUrl url = LatitudeService::locationHistoryUrl(d->granularity, d->maxResults, d->maxTimestamp, d->minTimestamp);
    const QNetworkRequest request = d->createRequest(url);

    enqueueRequest(request);
}

ObjectsList LocationFetchHistoryJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << LatitudeService::parseLocationJSONFeed(rawData, feedData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "moc_locationfetchhistoryjob.cpp"
