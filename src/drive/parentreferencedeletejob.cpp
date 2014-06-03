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


#include "parentreferencedeletejob.h"
#include "account.h"
#include "parentreference.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class ParentReferenceDeleteJob::Private
{
  public:
    QString fileId;
    QStringList referencesIds;
};

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId,
                                                   const QString &referenceId,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << referenceId;
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId,
                                                   const QStringList &referencesIds,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << referencesIds;
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId,
                                                   const ParentReferencePtr &reference,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << reference->id();
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId,
                                                   const ParentReferencesList &references,
                                                   const AccountPtr &account,
                                                   QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    Q_FOREACH(const ParentReferencePtr & reference, references) {
        d->referencesIds << reference->id();
    }
}

ParentReferenceDeleteJob::~ParentReferenceDeleteJob()
{
    delete d;
}

void ParentReferenceDeleteJob::start()
{
    if (d->referencesIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString referenceId = d->referencesIds.takeFirst();
    const QUrl url = DriveService::deleteParentReferenceUrl(d->fileId, referenceId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

#include "parentreferencedeletejob.moc"
