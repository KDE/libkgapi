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
using namespace KGAPI2::Drive;

class RevisionModifyJob::Private
{
  public:
    Private(RevisionModifyJob *parent);
    void processNext();

    QString fileId;
    RevisionsList revisions;

  private:
    RevisionModifyJob *q;
};

RevisionModifyJob::Private::Private(RevisionModifyJob *parent):
    q(parent)
{
}

void RevisionModifyJob::Private::processNext()
{
    if (revisions.isEmpty()) {
        q->emitFinished();
        return;
    }

    const RevisionPtr revision = revisions.takeFirst();
    const QUrl url = DriveService::modifyRevisionUrl(fileId, revision->id());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    const QByteArray rawData = Revision::toJSON(revision);
    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
}

RevisionModifyJob::RevisionModifyJob(const QString &fileId,
                                     const RevisionPtr &revision,
                                     const AccountPtr &account,
                                     QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revision;
}

RevisionModifyJob::RevisionModifyJob(const QString &fileId,
                                     const RevisionsList &revisions,
                                     const AccountPtr &account, QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->revisions << revisions;
}

RevisionModifyJob::~RevisionModifyJob()
{
    delete d;
}

void RevisionModifyJob::start()
{
    d->processNext();
}

ObjectsList RevisionModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                    const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << Revision::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}

#include "revisionmodifyjob.moc"

