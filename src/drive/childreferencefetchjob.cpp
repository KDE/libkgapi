/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "childreferencefetchjob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReferenceFetchJob::Private
{
public:
    Private(ChildReferenceFetchJob *parent);

    QString folderId;
    QString childId;

private:
    ChildReferenceFetchJob *const q;
};

ChildReferenceFetchJob::Private::Private(ChildReferenceFetchJob *parent)
    : q(parent)
{
}

ChildReferenceFetchJob::ChildReferenceFetchJob(const QString &folderId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
}

ChildReferenceFetchJob::ChildReferenceFetchJob(const QString &folderId, const QString &childId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private(this))
{
    d->folderId = folderId;
    d->childId = childId;
}

ChildReferenceFetchJob::~ChildReferenceFetchJob()
{
    delete d;
}

void ChildReferenceFetchJob::start()
{
    QUrl url;
    if (d->childId.isEmpty()) {
        url = DriveService::fetchChildReferences(d->folderId);
    } else {
        url = DriveService::fetchParentReferenceUrl(d->folderId, d->childId);
    }

    QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList ChildReferenceFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;
    FeedData feedData;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->childId.isEmpty()) {
            items << ChildReference::fromJSONFeed(rawData, feedData);
        } else {
            items << ChildReference::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        QNetworkRequest request(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}
