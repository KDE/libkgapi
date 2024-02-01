/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "locationdeletejob.h"
#include "latitudeservice.h"
#include "location.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN LocationDeleteJob::Private
{
public:
    Private();

    qulonglong locationId = 0;
    bool finished = false;
};

LocationDeleteJob::Private::Private()
{
}

LocationDeleteJob::LocationDeleteJob(const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
}

LocationDeleteJob::LocationDeleteJob(const LocationPtr &location, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->locationId = location->timestamp();
}

LocationDeleteJob::LocationDeleteJob(qulonglong timestamp, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->locationId = timestamp;
}

LocationDeleteJob::~LocationDeleteJob()
{
    delete d;
}

void LocationDeleteJob::start()
{
    if (d->finished) {
        emitFinished();
        return;
    }

    QUrl url;
    if (d->locationId > 0) {
        url = LatitudeService::deleteLocationUrl(d->locationId);
    } else {
        url = LatitudeService::deleteCurrentLocationUrl();
    }

    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1StringView(str) + QLatin1StringView(": ") + QLatin1StringView(request.rawHeader(str));
    }

    enqueueRequest(request);
    d->finished = true;
}

#include "moc_locationdeletejob.cpp"
