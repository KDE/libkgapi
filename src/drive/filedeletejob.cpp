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
using namespace KGAPI2::Drive;

class FileDeleteJob::Private
{
  public:
    QStringList filesIDs;
};

FileDeleteJob::FileDeleteJob(const QString &fileId,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << fileId;
}

FileDeleteJob::FileDeleteJob(const QStringList &filesIds,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << filesIds;
}

FileDeleteJob::FileDeleteJob(const FilePtr &file,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << file->id();
}

FileDeleteJob::FileDeleteJob(const FilesList &files,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    Q_FOREACH(const FilePtr & file, files) {
        d->filesIDs << file->id();
    }
}

FileDeleteJob::~FileDeleteJob()
{
    delete d;
}

void FileDeleteJob::start()
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


