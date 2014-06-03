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
#include "revisionfetchjob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"
#include "utils.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <KDE/KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class RevisionFetchJob::Private
{
  public:
    QString fileId;
    QString revisionId;
};

RevisionFetchJob::RevisionFetchJob(const QString &fileId,
                                   const AccountPtr &account,
                                   QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

RevisionFetchJob::RevisionFetchJob(const QString &fileId,
                                   const QString &revisionId,
                                   const AccountPtr &account,
                                   QObject *parent):
    FetchJob(account, parent),
    d(new Private)
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
    QNetworkRequest request;
    if (d->revisionId.isEmpty()) {
        request.setUrl(DriveService::fetchRevisionsUrl(d->fileId));
    } else {
        request.setUrl(DriveService::fetchRevisionUrl(d->fileId, d->revisionId));
    }
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

ObjectsList RevisionFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
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
        setErrorString(i18n("Invalid response content type"));
    }

    emitFinished();
    return items;
}

#include "revisionfetchjob.moc"
