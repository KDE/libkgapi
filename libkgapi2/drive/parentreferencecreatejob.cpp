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

#include "parentreferencecreatejob.h"
#include "account.h"
#include "driveservice.h"
#include "parentreference.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class DriveParentReferenceCreateJob::Private
{
  public:
    Private(DriveParentReferenceCreateJob *parent);
    void processNext();

    QString fileId;
    DriveParentReferencesList references;

  private:
    DriveParentReferenceCreateJob *q;
};

DriveParentReferenceCreateJob::Private::Private(DriveParentReferenceCreateJob *parent):
    q(parent)
{
}

void DriveParentReferenceCreateJob::Private::processNext()
{
    if (references.isEmpty()) {
        q->emitFinished();
        return;
    }

    const DriveParentReferencePtr reference = references.takeFirst();
    const QUrl url = DriveService::createParentReferenceUrl(fileId);

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = DriveParentReference::toJSON(reference);
    q->enqueueRequest(request, rawData, QLatin1String("application/json"));
}

DriveParentReferenceCreateJob::DriveParentReferenceCreateJob(const QString &fileId,
                                                             const QString &parentId,
                                                             const AccountPtr &account,
                                                             QObject *parent): 
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << DriveParentReferencePtr(new DriveParentReference(parentId));
}

DriveParentReferenceCreateJob::DriveParentReferenceCreateJob(const QString &fileId,
                                                             const QStringList &parentsIds,
                                                             const AccountPtr &account,
                                                             QObject *parent): 
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    Q_FOREACH (const QString &parentId, parentsIds) {
        d->references << DriveParentReferencePtr(new DriveParentReference(parentId));
    }
}

DriveParentReferenceCreateJob::DriveParentReferenceCreateJob(const QString &fileId,
                                                             const DriveParentReferencePtr &reference,
                                                             const AccountPtr &account,
                                                             QObject *parent): 
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << reference;
}

DriveParentReferenceCreateJob::DriveParentReferenceCreateJob(const QString &fileId,
                                                             const DriveParentReferencesList &references,
                                                             const AccountPtr &account,
                                                             QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->fileId = fileId;
    d->references << references;
}

DriveParentReferenceCreateJob::~DriveParentReferenceCreateJob()
{
    delete d;
}

void DriveParentReferenceCreateJob::start()
{
    d->processNext();
}

KGAPI2::ObjectsList DriveParentReferenceCreateJob::handleReplyWithItems(const QNetworkReply *reply,
                                                                        const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << DriveParentReference::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}

#include "parentreferencecreatejob.moc"
