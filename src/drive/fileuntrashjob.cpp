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


#include "fileuntrashjob.h"
#include "driveservice.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileUntrashJob::Private
{
};

FileUntrashJob::FileUntrashJob(const QString &fileId,
                               const AccountPtr &account,
                               QObject *parent):
    FileAbstractModifyJob(fileId, account, parent),
    d(new Private)
{
}

FileUntrashJob::FileUntrashJob(const QStringList &filesIds,
                               const AccountPtr &account,
                               QObject *parent):
    FileAbstractModifyJob(filesIds, account, parent),
    d(new Private)
{
}

FileUntrashJob::FileUntrashJob(const FilePtr &file,
                               const AccountPtr &account,
                               QObject *parent):
    FileAbstractModifyJob(file, account, parent),
    d(new Private)
{
}

FileUntrashJob::FileUntrashJob(const FilesList &files,
                               const AccountPtr &account,
                               QObject *parent):
    FileAbstractModifyJob(files, account, parent),
    d(new Private)
{
}

FileUntrashJob::~FileUntrashJob()
{
    delete d;
}

QUrl FileUntrashJob::url(const QString &fileId)
{
    return DriveService::untrashFileUrl(fileId);
}

void FileUntrashJob::dispatchRequest(QNetworkAccessManager *accessManager,
                                     const QNetworkRequest &request,
                                     const QByteArray &data,
                                     const QString &contentType)
{
    QNetworkRequest r(request);
    r.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    r.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    accessManager->post(r, data);
}


