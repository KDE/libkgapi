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


#include "permissiondeletejob.h"
#include "permission.h"
#include "account.h"
#include "driveservice.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class PermissionDeleteJob::Private
{
  public:
    QString fileId;
    QStringList permissionsIds;
};

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId,
                                         const PermissionPtr &permission,
                                         const AccountPtr &account,
                                         QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
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
    d->fileId = fileId;
    Q_FOREACH(const PermissionPtr & permission, permissions) {
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
    d->fileId = fileId;
    d->permissionsIds << permissionsIds;
}

PermissionDeleteJob::~PermissionDeleteJob()
{
    delete d;
}

void PermissionDeleteJob::start()
{
    if (d->permissionsIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString permissionId = d->permissionsIds.takeFirst();
    const QUrl url = DriveService::deletePermissionUrl(d->fileId, permissionId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

#include "permissiondeletejob.moc"
