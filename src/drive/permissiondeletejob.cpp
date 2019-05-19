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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "permissiondeletejob.h"
#include "permission.h"
#include "account.h"
#include "driveservice.h"

#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN PermissionDeleteJob::Private
{
  public:
    QString fileId;
    QStringList permissionsIds;
    bool supportsAllDrives;
};

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId,
                                         const PermissionPtr &permission,
                                         const AccountPtr &account,
                                         QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->fileId = fileId;
    d->permissionsIds << permission->id();
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId,
                                         const QString &permissionId,
                                         const AccountPtr &account,
                                         QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->fileId = fileId;
    d->permissionsIds << permissionId;
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId,
                                         const PermissionsList &permissions,
                                         const AccountPtr &account,
                                         QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->fileId = fileId;
    for (const PermissionPtr & permission : qAsConst(permissions)) {
        d->permissionsIds << permission->id();
    }
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId,
                                         const QStringList &permissionsIds,
                                         const AccountPtr &account,
                                         QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->fileId = fileId;
    d->permissionsIds << permissionsIds;
}

PermissionDeleteJob::~PermissionDeleteJob()
{
    delete d;
}

bool PermissionDeleteJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void PermissionDeleteJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

void PermissionDeleteJob::start()
{
    if (d->permissionsIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString permissionId = d->permissionsIds.takeFirst();
    QUrl url = DriveService::deletePermissionUrl(d->fileId, permissionId);
    QUrlQuery withDriveSupportQuery(url);
    withDriveSupportQuery.addQueryItem(QStringLiteral("supportsAllDrives"), d->supportsAllDrives ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(withDriveSupportQuery);
    QNetworkRequest request(url);

    enqueueRequest(request);
}


