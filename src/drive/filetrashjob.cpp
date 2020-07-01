/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "filetrashjob.h"
#include "driveservice.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileTrashJob::Private
{
};

FileTrashJob::FileTrashJob(const QString &fileId,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(fileId, account, parent),
    d(new Private)
{
}

FileTrashJob::FileTrashJob(const QStringList &filesIds,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(filesIds, account, parent),
    d(new Private)
{
}

FileTrashJob::FileTrashJob(const FilePtr &file,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(file, account, parent),
    d(new Private)
{
}

FileTrashJob::FileTrashJob(const FilesList &files,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(files, account, parent),
    d(new Private)
{
}

FileTrashJob::~FileTrashJob()
{
    delete d;
}

QUrl FileTrashJob::url(const QString &fileId)
{
    return DriveService::trashFileUrl(fileId);
}

void FileTrashJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                   const QNetworkRequest &request,
                                   const QByteArray &data,
                                   const QString &contentType)
{
    QNetworkRequest r(request);
    r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    r.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    accessManager->post(r, data);
}



