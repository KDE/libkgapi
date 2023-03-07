/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "parentreferencedeletejob.h"
#include "driveservice.h"
#include "parentreference.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN ParentReferenceDeleteJob::Private
{
public:
    QString fileId;
    QStringList referencesIds;
};

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId, const QString &referenceId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << referenceId;
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId, const QStringList &referencesIds, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << referencesIds;
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId, const ParentReferencePtr &reference, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
    d->referencesIds << reference->id();
}

ParentReferenceDeleteJob::ParentReferenceDeleteJob(const QString &fileId, const ParentReferencesList &references, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->fileId = fileId;
    for (const ParentReferencePtr &reference : references) {
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

    enqueueRequest(request);
}
