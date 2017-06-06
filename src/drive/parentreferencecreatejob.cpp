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

#include "parentreferencecreatejob.h"
#include "account.h"
#include "driveservice.h"
#include "parentreference.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ParentReferenceCreateJob::Private
{
  public:
    Private(ParentReferenceCreateJob *parent);
    void processNext();

    QString fileId;
    ParentReferencesList references;

private:
    ParentReferenceCreateJob *q;
};

ParentReferenceCreateJob::Private::Private(ParentReferenceCreateJob *parent):
    q(parent)
{
}

void ParentReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const ParentReferencePtr reference = references.takeFirst();
    const QUrl url = DriveService::createParentReferenceUrl(fileId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = ParentReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId,
                                                   const QString &parentId,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << ParentReferencePtr(new ParentReference(parentId));
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId,
                                                   const QStringList &parentsIds,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    for (const QString & parentId : qAsConst(parentsIds)) {
        d->references << ParentReferencePtr(new ParentReference(parentId));
    }
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId,
                                                   const ParentReferencePtr &reference,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << reference;
}

ParentReferenceCreateJob::ParentReferenceCreateJob(const QString &fileId,
                                                   const ParentReferencesList &references,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << references;
}

ParentReferenceCreateJob::~ParentReferenceCreateJob()
{
    delete d;
}

void ParentReferenceCreateJob::start()
{
    d->processNext();
}

ObjectsList ParentReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply,
                                                           const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ParentReference::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


