/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "childreferencedeletejob.h"
#include "account.h"
#include "childreference.h"
#include "driveservice.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ChildReferenceDeleteJob::Private
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
    for (const ChildReferencePtr & reference : references) {
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
        emitResult();
        return;
    }

    const QString childId = d->childrenIds.takeFirst();
    const QUrl url = DriveService::deleteChildReference(d->folderId, childId);

    QNetworkRequest request(url);

    enqueueRequest(request);
}


