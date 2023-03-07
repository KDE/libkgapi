/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "appfetchjob.h"
#include "app.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN AppFetchJob::Private
{
public:
    QString appId;
};

AppFetchJob::AppFetchJob(const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
}

AppFetchJob::AppFetchJob(const QString &appId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
    d->appId = appId;
}

AppFetchJob::~AppFetchJob()
{
    delete d;
}

void AppFetchJob::start()
{
    QUrl url;
    if (d->appId.isEmpty()) {
        url = DriveService::fetchAppsUrl();
    } else {
        url = DriveService::fetchAppUrl(d->appId);
    }
    QNetworkRequest request(url);

    enqueueRequest(request);
}

ObjectsList AppFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->appId.isEmpty()) {
            items << App::fromJSONFeed(rawData);
        } else {
            items << App::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}
