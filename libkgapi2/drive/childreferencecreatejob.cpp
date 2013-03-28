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

#include "childreferencecreatejob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class ChildReferenceCreateJob::Private
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
    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
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
    Q_FOREACH(const QString & childId, childrenIds) {
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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}

#include "childreferencecreatejob.moc"
