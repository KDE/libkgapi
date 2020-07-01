/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    enqueueRequest(request);
}


