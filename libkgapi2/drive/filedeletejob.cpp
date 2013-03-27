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

#include "filedeletejob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class DriveFileDeleteJob::Private
{
  public:
    QStringList filesIDs;
};

DriveFileDeleteJob::DriveFileDeleteJob(const QString &fileId,
                                       const AccountPtr &account,
                                       QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << fileId;
}

DriveFileDeleteJob::DriveFileDeleteJob(const QStringList &filesIds,
                                       const AccountPtr &account,
                                       QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << filesIds;
}

DriveFileDeleteJob::DriveFileDeleteJob(const DriveFilePtr &file,
                                       const AccountPtr &account,
                                       QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << file->id();
}

DriveFileDeleteJob::DriveFileDeleteJob(const DriveFilesList &files,
                                       const AccountPtr &account,
                                       QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    Q_FOREACH (const DriveFilePtr &file, files) {
        d->filesIDs << file->id();
    }
}

DriveFileDeleteJob::~DriveFileDeleteJob()
{
    delete d;
}

void DriveFileDeleteJob::start()
{
    if (d->filesIDs.isEmpty()) {
        emitFinished();
        return;
    }

    const QString fileId = d->filesIDs.takeFirst();
    const QUrl url = DriveService::deleteFileUrl(fileId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());

    enqueueRequest(request);
}

#include "filedeletejob.moc"
