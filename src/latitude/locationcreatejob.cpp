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

#include "locationcreatejob.h"
#include "latitudeservice.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


using namespace KGAPI2;

class LocationCreateJob::Private
{
  public:
    Private();

    LocationPtr location;
    bool isCurrent;
};

LocationCreateJob::Private::Private():
    isCurrent(true)
{
}

LocationCreateJob::LocationCreateJob(const LocationPtr& location, bool isCurrent, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->location = location;
    d->isCurrent = isCurrent;
}

LocationCreateJob::~LocationCreateJob()
{
    delete d;
}

void LocationCreateJob::start()
{
    QUrl url;
    if (d->isCurrent) {
        url = LatitudeService::insertCurrentLocationUrl();
    } else {
        url = LatitudeService::insertLocationUrl();
    }

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = LatitudeService::locationToJSON(d->location);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList LocationCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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


