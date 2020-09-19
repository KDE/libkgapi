/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    Latitude::Granularity granularity = Latitude::City;
    int maxResults = 0;
    qlonglong minTimestamp = 0;
    qlonglong maxTimestamp = 0;

  private:
    LocationFetchHistoryJob * const q;
};

LocationFetchHistoryJob::Private::Private(LocationFetchHistoryJob *parent):
    q(parent)
{
}

QNetworkRequest LocationFetchHistoryJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());

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
