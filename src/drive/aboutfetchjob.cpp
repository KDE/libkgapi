/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "aboutfetchjob.h"
#include "about.h"
#include "account.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"


#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN AboutFetchJob::Private
{
  public:
    Private();

    bool includeSubscribed;
    qlonglong maxChangeIdCount;
    qlonglong startChangeId;
};

AboutFetchJob::Private::Private():
    includeSubscribed(true),
    maxChangeIdCount(0),
    startChangeId(0)
{
}

AboutFetchJob::AboutFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
}

AboutFetchJob::~AboutFetchJob()
{
    delete d;
}

void AboutFetchJob::setIncludeSubscribed(bool includeSubscribed)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify includeSubscribed property when job is running";
        return;
    }

    d->includeSubscribed = includeSubscribed;
}

bool AboutFetchJob::includeSubscribed() const
{
    return d->includeSubscribed;
}

void AboutFetchJob::setMaxChangeIdCount(qlonglong maxChangeIdCount)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxChangeIdCount property when job is running";
        return;
    }

    d->maxChangeIdCount = maxChangeIdCount;
}

qlonglong AboutFetchJob::maxChangeIdCount() const
{
    return d->maxChangeIdCount;
}

void AboutFetchJob::setStartChangeId(qlonglong startChangeId)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify startChangeId property when job is running";
        return;
    }

    d->startChangeId = startChangeId;
}

qlonglong AboutFetchJob::startChangeId() const
{
    return d->startChangeId;
}

AboutPtr AboutFetchJob::aboutData() const
{
    if (isRunning() || items().count() == 0) {
        return AboutPtr();
    }

    return items().at(0).dynamicCast<About>();
}

void AboutFetchJob::start()
{
    QUrl url = DriveService::fetchAboutUrl(d->includeSubscribed, d->maxChangeIdCount, d->startChangeId);
    QNetworkRequest request(url);

    enqueueRequest(request);
}

KGAPI2::ObjectsList AboutFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                        const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        AboutPtr about = About::fromJSON(rawData);
        items << about;
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}


