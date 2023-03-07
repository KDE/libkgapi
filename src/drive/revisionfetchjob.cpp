/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "revisionfetchjob.h"
#include "driveservice.h"
#include "revision.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN RevisionFetchJob::Private
{
public:
    QString fileId;
    QString revisionId;
};

RevisionFetchJob::RevisionFetchJob(const QString &fileId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
}

RevisionFetchJob::RevisionFetchJob(const QString &fileId, const QString &revisionId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
    d->revisionId = revisionId;
}

RevisionFetchJob::~RevisionFetchJob()
{
    delete d;
}

void RevisionFetchJob::start()
{
    QUrl url;
    if (d->revisionId.isEmpty()) {
        url = DriveService::fetchRevisionsUrl(d->fileId);
    } else {
        url = DriveService::fetchRevisionUrl(d->fileId, d->revisionId);
    }
    QNetworkRequest request(url);

    enqueueRequest(request);
}

ObjectsList RevisionFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->revisionId.isEmpty()) {
            items << Revision::fromJSONFeed(rawData);
        } else {
            items << Revision::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}
