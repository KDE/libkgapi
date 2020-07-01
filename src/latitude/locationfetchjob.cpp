/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "locationfetchjob.h"
#include "latitudeservice.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN LocationFetchJob::Private
{
  public:
    Private();

    qlonglong timestamp;
    Latitude::Granularity granularity;
};

LocationFetchJob::Private::Private():
    timestamp(-1),
    granularity(Latitude::City)
{
}

LocationFetchJob::LocationFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private)
{
}

LocationFetchJob::LocationFetchJob(qlonglong timestamp, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->timestamp = timestamp;
}

LocationFetchJob::~LocationFetchJob()
{
    delete d;
}

Latitude::Granularity LocationFetchJob::granularity() const
{
    return d->granularity;
}

void LocationFetchJob::setGranularity(Latitude::Granularity granularity)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify granularity property when the job is running";
    }
    d->granularity = granularity;
}

void LocationFetchJob::start()
{
    QUrl url;
    if (d->timestamp == -1) {
        url = LatitudeService::retrieveCurrentLocationUrl(d->granularity);
    } else {
        url = LatitudeService::retrieveLocationUrl(d->timestamp, d->granularity);
    }
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());

    enqueueRequest(request);
}

ObjectsList LocationFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << LatitudeService::JSONToLocation(rawData).dynamicCast<Object>();
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    return items;
}

#include "moc_locationfetchjob.cpp"
