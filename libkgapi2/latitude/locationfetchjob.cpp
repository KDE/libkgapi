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

#include "locationfetchjob.h"
#include "latitudeservice.h"
#include "account.h"
#include "debug.h"
#include "location.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class LocationFetchJob::Private
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
        KGAPIWarning() << "Can't modify granularity property when the job is running";
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
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());
    request.setUrl(url);

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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    return items;
}

#include "locationfetchjob.moc"
