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

#include "filemodifyjob.h"
#include "file.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileModifyJob::Private
{
  public:
    Private();

    QMap < QString /* filepath */, QString /* fileId */ > files;

    bool createNewRevision;
    bool changeModifiedDate;
    bool updateViewedDate;
};

FileModifyJob::Private::Private():
    createNewRevision(true),
    changeModifiedDate(false),
    updateViewedDate(true)
{
}

FileModifyJob::FileModifyJob(const FilePtr &metadata,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(metadata, account, parent),
    d(new Private)
{
    d->files.insert(QLatin1String("?=0"), metadata->id());
}

FileModifyJob::FileModifyJob(const QString &filePath,
                             const QString &fileId,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
    d->files.insert(filePath, fileId);
}

FileModifyJob::FileModifyJob(const QString &filePath,
                             const FilePtr &metaData,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
    d->files.insert(filePath, metaData->id());
}

FileModifyJob::FileModifyJob(const QMap< QString, QString > &files,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(files.keys(), account, parent),
    d(new Private)
{
    d->files = files;
}

FileModifyJob::FileModifyJob(const QMap< QString, FilePtr > &files,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractUploadJob(files, account, parent),
    d(new Private)
{
    QMap<QString, FilePtr>::ConstIterator iter = files.constBegin();
    QMap<QString, FilePtr>::ConstIterator iterEnd = files.constEnd();
    for (; iter != iterEnd; ++iter) {
        d->files.insert(iter.key(), iter.value()->id());
    }
}

FileModifyJob::~FileModifyJob()
{
    delete d;
}

bool FileModifyJob::createNewRevision() const
{
    return d->createNewRevision;
}

void FileModifyJob::setCreateNewRevision(bool createNewRevision)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify createNewRevision property when the job is running";
        return;
    }

    d->createNewRevision = createNewRevision;
}

bool FileModifyJob::updateModifiedDate() const
{
    return d->updateViewedDate;
}

void FileModifyJob::setUpdateModifiedDate(bool updateModifiedDate)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify updateModifiedDate property when the job is running";
        return;
    }

    d->updateViewedDate = updateModifiedDate;
}

bool FileModifyJob::updateViewedDate() const
{
    return d->updateViewedDate;
}

void FileModifyJob::setUpdateViewedDate(bool updateViewedDate)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify updateViewedDate property when job is running";
        return;
    }

    d->updateViewedDate = updateViewedDate;
}

QUrl FileModifyJob::createUrl(const QString &filePath,
                              const FilePtr &metaData)
{
    QUrl url;

    if (metaData.isNull()) {
        url = DriveService::uploadMediaFileUrl(d->files.value(filePath));
    } else if (filePath.isEmpty()) {
        url = DriveService::uploadMetadataFileUrl(metaData->id());
    } else {
        url = DriveService::uploadMultipartFileUrl(d->files.value(filePath));
    }

    url.addQueryItem(QLatin1String("newRevision"), Utils::bool2Str(d->createNewRevision));
    url.addQueryItem(QLatin1String("setModifiedDate"), Utils::bool2Str(d->changeModifiedDate));
    url.addQueryItem(QLatin1String("updateViewedDate"), Utils::bool2Str(d->updateViewedDate));

    return url;
}

QNetworkReply *FileModifyJob::dispatch(QNetworkAccessManager *accessManager,
                                       const QNetworkRequest &request,
                                       const QByteArray &data)
{
    return accessManager->put(request, data);
}



