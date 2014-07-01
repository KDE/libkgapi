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

#include "revisiondeletejob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class RevisionDeleteJob::Private
{
  public:
    QString fileId;
    QStringList revisionsIds;
};

RevisionDeleteJob::RevisionDeleteJob(const QString &fileId,
                                     const QString &revisionId,
                                     const AccountPtr &account,
                                     QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->revisionsIds << revisionId;
}

RevisionDeleteJob::RevisionDeleteJob(const QString &fileId,
                                     const QStringList &revisionsIds,
                                     const AccountPtr &account,
                                     QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->revisionsIds << revisionsIds;
}

RevisionDeleteJob::RevisionDeleteJob(const QString &fileId,
                                     const RevisionPtr &revision,
                                     const AccountPtr &account,
                                     QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->revisionsIds << revision->id();
}

RevisionDeleteJob::RevisionDeleteJob(const QString &fileId,
                                     const RevisionsList &revisions,
                                     const AccountPtr &account,
                                     QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    Q_FOREACH(const RevisionPtr & revision, revisions) {
        d->revisionsIds << revision->id();
    }
}

RevisionDeleteJob::~RevisionDeleteJob()
{
    delete d;
}

void RevisionDeleteJob::start()
{
    if (d->revisionsIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString revisionId = d->revisionsIds.takeFirst();
    const QUrl url = DriveService::deleteRevisionUrl(d->fileId, revisionId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}


