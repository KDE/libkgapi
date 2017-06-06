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

#include "locationdeletejob.h"
#include "latitudeservice.h"
#include "account.h"
#include "../debug.h"
#include "location.h"
#include "utils.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN LocationDeleteJob::Private
{
  public:
    Private();

    qulonglong locationId;
    bool finished;
};

LocationDeleteJob::Private::Private():
    locationId(0),
    finished(false)
{
}

LocationDeleteJob::LocationDeleteJob(const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
}

LocationDeleteJob::LocationDeleteJob(const LocationPtr& location,
                                     const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->locationId = location->timestamp();
}

LocationDeleteJob::LocationDeleteJob(qulonglong timestamp,
                                     const AccountPtr &account,
                                     QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
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
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", LatitudeService::APIVersion().toLatin1());

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    enqueueRequest(request);
    d->finished = true;
}


