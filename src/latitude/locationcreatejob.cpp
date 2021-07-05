/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    bool isCurrent = true;
};

LocationCreateJob::Private::Private()
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
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
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


