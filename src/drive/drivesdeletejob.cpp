/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "drivesdeletejob.h"
#include "drives.h"
#include "account.h"
#include "driveservice.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN DrivesDeleteJob::Private
{
  public:
    QStringList drivesIds;
};

DrivesDeleteJob::DrivesDeleteJob(const QString &drivesId,
                                         const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->drivesIds << drivesId;
}

DrivesDeleteJob::DrivesDeleteJob(const QStringList &drivesIds,
                                         const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->drivesIds << drivesIds;
}

DrivesDeleteJob::DrivesDeleteJob(const DrivesPtr &drives,
                                         const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->drivesIds << drives->id();
}

DrivesDeleteJob::DrivesDeleteJob(const DrivesList &drives,
                                         const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    for (const DrivesPtr & drives : std::as_const(drives)) {
        d->drivesIds << drives->id();
    }
}

DrivesDeleteJob::~DrivesDeleteJob() = default;

void DrivesDeleteJob::start()
{
    if (d->drivesIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString drivesId = d->drivesIds.takeFirst();
    const QUrl url = DriveService::fetchDrivesUrl(drivesId);

    QNetworkRequest request(url);
    enqueueRequest(request);
}


