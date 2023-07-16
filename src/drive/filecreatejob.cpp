/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "filecreatejob.h"
#include "driveservice.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileCreateJob::Private
{
};

FileCreateJob::FileCreateJob(const FilePtr &metadata, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(metadata, account, parent)
    , d(new Private)
{
}

FileCreateJob::FileCreateJob(const FilesList &metadata, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(metadata, account, parent)
    , d(new Private)
{
}

FileCreateJob::FileCreateJob(const QString &filePath, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(filePath, account, parent)
    , d(new Private)
{
}

FileCreateJob::FileCreateJob(const QString &filePath, const FilePtr &metaData, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(filePath, metaData, account, parent)
    , d(new Private)
{
}

FileCreateJob::FileCreateJob(const QStringList &filePaths, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(filePaths, account, parent)
    , d(new Private)
{
}

FileCreateJob::FileCreateJob(const QMap<QString, FilePtr> &files, const AccountPtr &account, QObject *parent)
    : FileAbstractUploadJob(files, account, parent)
    , d(new Private)
{
}

FileCreateJob::~FileCreateJob()
{
    delete d;
}

QNetworkReply *FileCreateJob::dispatch(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data)
{
    return accessManager->post(request, data);
}

QUrl FileCreateJob::createUrl(const QString &filePath, const FilePtr &metaData)
{
    if (filePath.isEmpty() && !metaData.isNull()) {
        return DriveService::uploadMetadataFileUrl();
    } else if (metaData.isNull()) {
        return DriveService::uploadMediaFileUrl();
    } else {
        return DriveService::uploadMultipartFileUrl();
    }
}

#include "moc_filecreatejob.cpp"
