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

#include "childreferencefetchjob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveChildReferenceFetchJob::Private
{
  public:
    Private(DriveChildReferenceFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    QString folderId;
    QString childId;

  private:
    DriveChildReferenceFetchJob *q;
};

DriveChildReferenceFetchJob::Private::Private(DriveChildReferenceFetchJob *parent):
    q(parent)
{
}

QNetworkRequest DriveChildReferenceFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    return request;
}

DriveChildReferenceFetchJob::DriveChildReferenceFetchJob(const QString &folderId,
                                                         const AccountPtr &account,
                                                         QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
}

DriveChildReferenceFetchJob::DriveChildReferenceFetchJob(const QString &folderId,
                                                         const QString &childId,
                                                         const AccountPtr &account,
                                                         QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->childId = childId;
}

DriveChildReferenceFetchJob::~DriveChildReferenceFetchJob()
{
    delete d;
}

void DriveChildReferenceFetchJob::start()
{
    QUrl url;
    if (d->childId.isEmpty()) {
        url = DriveService::fetchChildReferences(d->folderId);
    } else {
        url = DriveService::fetchParentReferenceUrl(d->folderId, d->childId);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList DriveChildReferenceFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                              const QByteArray &rawData)
{
    ObjectsList items;
    FeedData feedData;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->childId.isEmpty()) {
            items << DriveChildReference::fromJSONFeed(rawData, feedData);
        } else {
            items << DriveChildReference::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }


    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "childreferencefetchjob.moc"
