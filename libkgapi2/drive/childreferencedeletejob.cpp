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

#include "childreferencedeletejob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class ChildReferenceDeleteJob::Private
{
  public:
    QString folderId;
    QStringList childrenIds;
};

ChildReferenceDeleteJob::ChildReferenceDeleteJob(const QString &folderId,
                                                 const QString &childId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->folderId = folderId;
    d->childrenIds << childId;
}

ChildReferenceDeleteJob::ChildReferenceDeleteJob(const QString &folderId,
                                                 const QStringList &childrenIds,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->folderId = folderId;
    d->childrenIds << childrenIds;
}

ChildReferenceDeleteJob::ChildReferenceDeleteJob(const QString &folderId,
                                                 const ChildReferencePtr &reference,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->folderId = folderId;
    d->childrenIds << reference->id();
}

ChildReferenceDeleteJob::ChildReferenceDeleteJob(const QString &folderId,
                                                 const ChildReferencesList &references,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->folderId = folderId;
    Q_FOREACH(const ChildReferencePtr & reference, references) {
        d->childrenIds << reference->id();
    }
}

ChildReferenceDeleteJob::~ChildReferenceDeleteJob()
{
    delete d;
}

void ChildReferenceDeleteJob::start()
{
    if (d->childrenIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString childId = d->childrenIds.takeFirst();
    const QUrl url = DriveService::deleteChildReference(d->folderId, childId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

#include "childreferencedeletejob.moc"
