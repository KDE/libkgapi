/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2019 David Barchiesi <david@barchie.si>
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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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
    for (const DrivesPtr & drives : qAsConst(drives)) {
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


