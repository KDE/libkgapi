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


#include "filecreatejob.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileCreateJob::Private
{

};

FileCreateJob::FileCreateJob(const FilePtr &metadata,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(metadata, account, parent),
    d(new Private)
{
}

FileCreateJob::FileCreateJob(const FilesList &metadata,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(metadata, account, parent),
    d(new Private)
{
}

FileCreateJob::FileCreateJob(const QString &filePath,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
}

FileCreateJob::FileCreateJob(const QString &filePath,
                             const FilePtr &metaData,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(filePath, metaData, account, parent),
    d(new Private)
{
}

FileCreateJob::FileCreateJob(const QStringList &filePaths,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(filePaths, account, parent),
    d(new Private)
{
}

FileCreateJob::FileCreateJob(const QMap< QString, FilePtr > &files,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(files, account, parent),
    d(new Private)
{
}

FileCreateJob::~FileCreateJob()
{
    delete d;
}

QNetworkReply *FileCreateJob::dispatch(QNetworkAccessManager *accessManager,
                                       const QNetworkRequest &request,
                                       const QByteArray &data)
{
    return accessManager->post(request, data);
}

QUrl FileCreateJob::createUrl(const QString &filePath,
                              const FilePtr &metaData)
{
    if (filePath.isEmpty() && !metaData.isNull()) {
        return DriveService::uploadMetadataFileUrl();
    } else if (metaData.isNull()) {
        return DriveService::uploadMediaFileUrl();
    } else {
        return DriveService::uploadMultipartFileUrl();
    }
}


