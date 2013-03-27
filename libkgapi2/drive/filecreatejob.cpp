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
#include "debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

using namespace KGAPI2;

class DriveFileCreateJob::Private
{

};

DriveFileCreateJob::DriveFileCreateJob(const QString &filePath,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
}

DriveFileCreateJob::DriveFileCreateJob(const QString &filePath,
                                       const DriveFilePtr &metaData,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(filePath, metaData, account, parent),
    d(new Private)
{
}

DriveFileCreateJob::DriveFileCreateJob(const QStringList &filePaths,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(filePaths, account, parent),
    d(new Private)
{
}

DriveFileCreateJob::DriveFileCreateJob(const QMap< QString, DriveFilePtr > &files,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(files, account, parent),
    d(new Private)
{
}

DriveFileCreateJob::~DriveFileCreateJob()
{
    delete d;
}

QNetworkReply* DriveFileCreateJob::dispatch(QNetworkAccessManager *accessManager,
                                            const QNetworkRequest &request,
                                            const QByteArray &data)
{
    return accessManager->post(request, data);
}

QUrl DriveFileCreateJob::createUrl(const QString &filePath,
                                   const DriveFilePtr &metaData)
{
    Q_UNUSED(filePath)

    if (metaData.isNull()) {
        return DriveService::uploadMediaFileUrl();
    } else {
        return DriveService::uploadMultipartFileUrl();
    }
}

#include "filecreatejob.moc"
