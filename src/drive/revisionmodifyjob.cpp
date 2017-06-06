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

#include "revisionmodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN RevisionModifyJob::Private
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
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
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
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}



