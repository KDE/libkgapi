/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "permissiondeletejob.h"
#include "driveservice.h"
#include "permission.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace
{
static constexpr bool useDomainAdminAccessDefault = false;
}

class Q_DECL_HIDDEN PermissionDeleteJob::Private
{
public:
    QString fileId;
    QStringList permissionsIds;
    bool supportsAllDrives;
    bool useDomainAdminAccess;
};

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId, const PermissionPtr &permission, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
    d->permissionsIds << permission->id();
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId, const QString &permissionId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
    d->permissionsIds << permissionId;
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId, const PermissionsList &permissions, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
    for (const PermissionPtr &permission : std::as_const(permissions)) {
        d->permissionsIds << permission->id();
    }
}

PermissionDeleteJob::PermissionDeleteJob(const QString &fileId, const QStringList &permissionsIds, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
    d->permissionsIds << permissionsIds;
}

PermissionDeleteJob::~PermissionDeleteJob() = default;

bool PermissionDeleteJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void PermissionDeleteJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

bool PermissionDeleteJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void PermissionDeleteJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    d->useDomainAdminAccess = useDomainAdminAccess;
}

void PermissionDeleteJob::start()
{
    if (d->permissionsIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString permissionId = d->permissionsIds.takeFirst();
    QUrl url = DriveService::deletePermissionUrl(d->fileId, permissionId);
    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(d->supportsAllDrives));
    if (d->useDomainAdminAccess != useDomainAdminAccessDefault) {
        query.addQueryItem(QStringLiteral("useDomainAdminAccess"), Utils::bool2Str(d->useDomainAdminAccess));
    }
    url.setQuery(query);
    QNetworkRequest request(url);

    enqueueRequest(request);
}
