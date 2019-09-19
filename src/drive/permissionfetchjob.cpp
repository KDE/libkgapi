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

#include "permissionfetchjob.h"
#include "driveservice.h"
#include "account.h"
#include "file.h"
#include "permission.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

namespace {
    static constexpr bool useDomainAdminAccessDefault = false;
}

class Q_DECL_HIDDEN PermissionFetchJob::Private
{
  public:
    QString fileId;
    QString permissionId;
    bool supportsAllDrives;
    bool useDomainAdminAccess;
};

PermissionFetchJob::PermissionFetchJob(const QString &fileId,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
}

PermissionFetchJob::PermissionFetchJob(const FilePtr &file,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = file->id();
}

PermissionFetchJob::PermissionFetchJob(const QString &fileId,
                                       const QString &permissionId,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = fileId;
    d->permissionId = permissionId;
}

PermissionFetchJob::PermissionFetchJob(const FilePtr &file,
                                       const QString &permissionId,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->supportsAllDrives = true;
    d->useDomainAdminAccess = useDomainAdminAccessDefault;
    d->fileId = file->id();
    d->permissionId = permissionId;
}

PermissionFetchJob::~PermissionFetchJob() = default;

bool PermissionFetchJob::supportsAllDrives() const
{
    return d->supportsAllDrives;
}

void PermissionFetchJob::setSupportsAllDrives(bool supportsAllDrives)
{
    d->supportsAllDrives = supportsAllDrives;
}

bool PermissionFetchJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void PermissionFetchJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    d->useDomainAdminAccess = useDomainAdminAccess;
}

void PermissionFetchJob::start()
{
    QUrl url;
    if (d->permissionId.isEmpty()) {
        url = DriveService::fetchPermissionsUrl(d->fileId);
    } else {
        url = DriveService::fetchPermissionUrl(d->fileId, d->permissionId);
    }

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("supportsAllDrives"), Utils::bool2Str(d->supportsAllDrives));
    if (d->useDomainAdminAccess != useDomainAdminAccessDefault) {
        query.addQueryItem(QStringLiteral("useDomainAdminAccess"), Utils::bool2Str(d->useDomainAdminAccess));
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList PermissionFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->permissionId.isEmpty()) {
            items << Permission::fromJSONFeed(rawData);
        } else {
            items << Permission::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
    }

    emitFinished();
    return items;
}


