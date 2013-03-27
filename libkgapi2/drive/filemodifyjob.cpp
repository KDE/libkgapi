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
#include "debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

using namespace KGAPI2;

class DriveFileModifyJob::Private
{
  public:
    Private();

    QMap<QString /* filepath */, QString /* fileId */ > files;

    bool createNewRevision;
    bool changeModifiedDate;
    bool updateViewedDate;
};

DriveFileModifyJob::Private::Private():
    createNewRevision(true),
    changeModifiedDate(false),
    updateViewedDate(true)
{
}

DriveFileModifyJob::DriveFileModifyJob(const QString &filePath,
                                       const QString &fileId,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
    d->files.insert(filePath, fileId);
}

DriveFileModifyJob::DriveFileModifyJob(const QString &filePath,
                                       const DriveFilePtr &metaData,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(filePath, account, parent),
    d(new Private)
{
    d->files.insert(filePath, metaData->id());
}

DriveFileModifyJob::DriveFileModifyJob(const QMap< QString, QString > &files,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(files.keys(), account, parent),
    d(new Private)
{
    d->files = files;
}

DriveFileModifyJob::DriveFileModifyJob(const QMap< QString, DriveFilePtr > &files,
                                       const AccountPtr &account,
                                       QObject *parent):
    DriveFileAbstractUploadJob(files, account, parent),
    d(new Private)
{
    Q_FOREACH (const QString &filePath, files.keys()) {
        d->files.insert(filePath, files.value(filePath)->id());
    }
}

DriveFileModifyJob::~DriveFileModifyJob()
{
    delete d;
}

bool DriveFileModifyJob::createNewRevision() const
{
    return d->createNewRevision;
}

void DriveFileModifyJob::setCreateNewRevision(bool createNewRevision)
{
    if (isRunning()) {
        kWarning() << "Can't modify createNewRevision property when the job is running";
        return;
    }

    d->createNewRevision = createNewRevision;
}

bool DriveFileModifyJob::updateModifiedDate() const
{
    return d->updateViewedDate;
}

void DriveFileModifyJob::setUpdateModifiedDate(bool updateModifiedDate)
{
    if (isRunning()) {
        kWarning() << "Can't modify updateModifiedDate property when the job is running";
        return;
    }

    d->updateViewedDate = updateModifiedDate;
}

bool DriveFileModifyJob::updateViewedDate() const
{
    return d->updateViewedDate;
}

void DriveFileModifyJob::setUpdateViewedDate(bool updateViewedDate)
{
    if (isRunning()) {
        kWarning() << "Can't modify updateViewedDate property when job is running";
        return;
    }

    d->updateViewedDate = updateViewedDate;
}

QUrl DriveFileModifyJob::createUrl(const QString &filePath,
                                   const DriveFilePtr &metaData)
{
    QUrl url;

    if (metaData.isNull()) {
        url = DriveService::uploadMediaFileUrl(d->files.value(filePath));
    } else {
        url = DriveService::uploadMultipartFileUrl(d->files.value(filePath));
    }

    url.addQueryItem(QLatin1String("newRevision"), Utils::bool2Str(d->createNewRevision));
    url.addQueryItem(QLatin1String("setModifiedDate"), Utils::bool2Str(d->changeModifiedDate));
    url.addQueryItem(QLatin1String("updateViewedDate"), Utils::bool2Str(d->updateViewedDate));

    return url;
}

QNetworkReply* DriveFileModifyJob::dispatch(QNetworkAccessManager *accessManager,
                                            const QNetworkRequest &request,
                                            const QByteArray &data)
{
    return accessManager->put(request, data);
}


#include "filemodifyjob.moc"
