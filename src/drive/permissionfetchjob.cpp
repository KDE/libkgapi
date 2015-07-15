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

#include "permissionfetchjob.h"
#include "driveservice.h"
#include "account.h"
#include "file.h"
#include "permission.h"
#include "utils.h"


#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class PermissionFetchJob::Private
{
  public:
    QString fileId;
    QString permissionId;

};

PermissionFetchJob::PermissionFetchJob(const QString &fileId,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

PermissionFetchJob::PermissionFetchJob(const FilePtr &file,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = file->id();
}

PermissionFetchJob::PermissionFetchJob(const QString &fileId,
                                       const QString &permissionId,
                                       const AccountPtr &account,
                                       QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
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
    d->fileId = file->id();
    d->permissionId = permissionId;
}

PermissionFetchJob::~PermissionFetchJob()
{
    delete d;
}

void PermissionFetchJob::start()
{
    QNetworkRequest request;
    if (d->permissionId.isEmpty()) {
        request.setUrl(DriveService::fetchPermissionsUrl(d->fileId));
    } else {
        request.setUrl(DriveService::fetchPermissionUrl(d->fileId, d->permissionId));
    }
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

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


