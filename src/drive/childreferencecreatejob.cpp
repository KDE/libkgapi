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

#include "childreferencecreatejob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReferenceCreateJob::Private
{
  public:
    Private(ChildReferenceCreateJob *parent);
    void processNext();

    QString folderId;
    ChildReferencesList references;

  private:
    ChildReferenceCreateJob *q;
};

ChildReferenceCreateJob::Private::Private(ChildReferenceCreateJob *parent):
    q(parent)
{
}

void ChildReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const ChildReferencePtr reference = references.takeFirst();
    const QUrl url = DriveService::createChildReference(folderId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = ChildReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId,
                                                 const QString &childId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << ChildReferencePtr(new ChildReference(childId));
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId,
                                                 const QStringList &childrenIds,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    for (const QString & childId : qAsConst(childrenIds)) {
        d->references << ChildReferencePtr(new ChildReference(childId));
    }
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId,
                                                 const ChildReferencePtr &reference,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << reference;
}

ChildReferenceCreateJob::ChildReferenceCreateJob(const QString &folderId,
                                                 const ChildReferencesList &references,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << references;
}

ChildReferenceCreateJob::~ChildReferenceCreateJob()
{
    delete d;
}

void ChildReferenceCreateJob::start()
{
    d->processNext();
}

ObjectsList ChildReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply,
                                                          const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << ChildReference::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


