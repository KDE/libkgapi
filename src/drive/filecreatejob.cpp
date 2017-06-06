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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "filecreatejob.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileCreateJob::Private
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


