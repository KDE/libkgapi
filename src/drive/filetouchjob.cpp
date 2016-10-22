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

#include "filetouchjob.h"
#include "driveservice.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileTouchJob::Private
{
};

FileTouchJob::FileTouchJob(const QString &fileId,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(fileId, account, parent),
    d(new Private)
{
}

FileTouchJob::FileTouchJob(const QStringList &filesIds,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(filesIds, account, parent),
    d(new Private)
{
}

FileTouchJob::FileTouchJob(const FilePtr &file,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(file, account, parent),
    d(new Private)
{
}

FileTouchJob::FileTouchJob(const FilesList &files,
                           const AccountPtr &account, QObject *parent):
    FileAbstractModifyJob(files, account, parent),
    d(new Private)
{
}

FileTouchJob::~FileTouchJob()
{
    delete d;
}

QUrl FileTouchJob::url(const QString &fileId)
{
    return DriveService::touchFileUrl(fileId);
}


