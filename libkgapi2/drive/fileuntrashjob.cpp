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


#include "fileuntrashjob.h"
#include "driveservice.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class FileUntrashJob::Private
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

#include "fileuntrashjob.moc"
