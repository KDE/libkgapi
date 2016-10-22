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

#include "locationfetchjob.h"
#include "latitudeservice.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"
#include "latitudeservice.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


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
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    return items;
}

#include "moc_locationfetchjob.cpp"
