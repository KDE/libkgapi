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

#include "childreferencefetchjob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReferenceFetchJob::Private
{
  public:
    Private(ChildReferenceFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    QString folderId;
    QString childId;

  private:
    ChildReferenceFetchJob *q;
};

ChildReferenceFetchJob::Private::Private(ChildReferenceFetchJob *parent):
    q(parent)
{
}

QNetworkRequest ChildReferenceFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    return request;
}

ChildReferenceFetchJob::ChildReferenceFetchJob(const QString &folderId,
                                               const AccountPtr &account,
                                               QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
}

ChildReferenceFetchJob::ChildReferenceFetchJob(const QString &folderId,
                                               const QString &childId,
                                               const AccountPtr &account,
                                               QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
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

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList ChildReferenceFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                         const QByteArray &rawData)
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
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


