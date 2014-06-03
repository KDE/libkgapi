/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "locationfetchhistoryjob.h"
#include "account.h"
#include "debug.h"
#include "location.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class LocationFetchHistoryJob::Private
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
        KGAPIWarning() << "Can't modify maxResults property while job is running";
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
        KGAPIWarning() << "Can't modify maxResults property while job is running";
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
        KGAPIWarning() << "Can't modify maxResults property while job is running";
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
        KGAPIWarning() << "Can't modify maxResults property while job is running";
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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "locationfetchhistoryjob.moc"
