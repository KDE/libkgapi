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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "locationcreatejob.h"
#include "latitudeservice.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN LocationCreateJob::Private
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

    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());

    const QByteArray rawData = LatitudeService::locationToJSON(d->location);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
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


