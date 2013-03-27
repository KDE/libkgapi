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
#include <qjson/parser.h>
#include <KLocalizedString>

using namespace KGAPI2;

class DrivePermissionFetchJob::Private
{
  public:
    QString fileId;
    QString permissionId;

};

DrivePermissionFetchJob::DrivePermissionFetchJob(const QString &fileId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

DrivePermissionFetchJob::DrivePermissionFetchJob(const DriveFilePtr &file,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = file->id();
}

DrivePermissionFetchJob::DrivePermissionFetchJob(const QString &fileId,
                                                 const QString &permissionId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
    d->permissionId = permissionId;
}

DrivePermissionFetchJob::DrivePermissionFetchJob(const DriveFilePtr &file,
                                                 const QString &permissionId,
                                                 const AccountPtr &account,
                                                 QObject *parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->fileId = file->id();
    d->permissionId = permissionId;
}

DrivePermissionFetchJob::~DrivePermissionFetchJob()
{
    delete d;
}

void DrivePermissionFetchJob::start()
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

ObjectsList DrivePermissionFetchJob::handleReplyWithItems(const QNetworkReply *reply,
                                                          const QByteArray &rawData)
{
    ObjectsList items;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->permissionId.isEmpty()) {
            items << DrivePermission::fromJSONFeed(rawData);
        } else {
            items << DrivePermission::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
    }

    emitFinished();
    return items;
}

#include "permissionfetchjob.moc"
