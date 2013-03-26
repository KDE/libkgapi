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

#include "revisionmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"
#include "utils.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveRevisionModifyJob::Private
{
  public:
    Private(DriveRevisionModifyJob *parent);
    void processNext();

    QString fileId;
    DriveRevisionsList revisions;

  private:
    DriveRevisionModifyJob *q;
};

DriveRevisionModifyJob::Private::Private(DriveRevisionModifyJob *parent):
    q(parent)
{
}

void DriveRevisionModifyJob::Private::processNext()
{
    if (revisions.isEmpty()) {
        q->emitFinished();
        return;
    }

    const DriveRevisionPtr revision = revisions.takeFirst();
    const QUrl url = DriveService::modifyRevisionUrl(fileId, revision->id());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    const QByteArray rawData = DriveRevision::toJSON(revision);
    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
}

DriveRevisionModifyJob::DriveRevisionModifyJob(const QString &fileId,
                                               const DriveRevisionPtr &revision,
                                               const AccountPtr &account,
                                               QObject *parent): 
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revision;
}

DriveRevisionModifyJob::DriveRevisionModifyJob(const QString &fileId, const DriveRevisionsList &revisions, const AccountPtr &account, QObject *parent): 
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revisions;
}

DriveRevisionModifyJob::~DriveRevisionModifyJob()
{
    delete d;
}

void DriveRevisionModifyJob::start()
{
    d->processNext();
}

ObjectsList DriveRevisionModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                         const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << DriveRevision::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}
