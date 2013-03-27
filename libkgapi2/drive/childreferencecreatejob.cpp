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

class DriveChildReferenceCreateJob::Private
{
  public:
    Private(DriveChildReferenceCreateJob *parent);
    void processNext();

    QString folderId;
    DriveChildReferencesList references;

  private:
    DriveChildReferenceCreateJob *q;
};

DriveChildReferenceCreateJob::Private::Private(DriveChildReferenceCreateJob *parent):
    q(parent)
{
}

void DriveChildReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const DriveChildReferencePtr reference = references.takeFirst();
    const QUrl url = DriveService::createChildReference(folderId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = DriveChildReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
}

DriveChildReferenceCreateJob::DriveChildReferenceCreateJob(const QString &folderId,
                                                           const QString &childId,
                                                           const AccountPtr &account,
                                                           QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << DriveChildReferencePtr(new DriveChildReference(childId));
}

DriveChildReferenceCreateJob::DriveChildReferenceCreateJob(const QString &folderId,
                                                           const QStringList &childrenIds,
                                                           const AccountPtr &account,
                                                           QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    Q_FOREACH (const QString &childId, childrenIds) {
        d->references << DriveChildReferencePtr(new DriveChildReference(childId));
    }
}

DriveChildReferenceCreateJob::DriveChildReferenceCreateJob(const QString &folderId,
                                                           const DriveChildReferencePtr &reference,
                                                           const AccountPtr &account,
                                                           QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << reference;
}

DriveChildReferenceCreateJob::DriveChildReferenceCreateJob(const QString &folderId,
                                                           const DriveChildReferencesList &references,
                                                           const AccountPtr &account,
                                                           QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->folderId = folderId;
    d->references << references;
}

DriveChildReferenceCreateJob::~DriveChildReferenceCreateJob()
{
    delete d;
}

void DriveChildReferenceCreateJob::start()
{
    d->processNext();
}

ObjectsList DriveChildReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply,
                                                               const QByteArray &rawData)
{
   const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << DriveChildReference::fromJSON(rawData);
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
