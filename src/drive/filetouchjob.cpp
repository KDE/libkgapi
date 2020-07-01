/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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


