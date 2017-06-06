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

#include "revisiondeletejob.h"
#include "account.h"
#include "driveservice.h"
#include "revision.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN RevisionDeleteJob::Private
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
    for (const RevisionPtr &revision : qAsConst(revisions)) {
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


