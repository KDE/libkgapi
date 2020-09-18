/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "filedeletejob.h"
#include "account.h"
#include "driveservice.h"
#include "file.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileDeleteJob::Private
{
  public:
    QStringList filesIDs;
};

FileDeleteJob::FileDeleteJob(const QString &fileId,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << fileId;
}

FileDeleteJob::FileDeleteJob(const QStringList &filesIds,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << filesIds;
}

FileDeleteJob::FileDeleteJob(const FilePtr &file,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->filesIDs << file->id();
}

FileDeleteJob::FileDeleteJob(const FilesList &files,
                             const AccountPtr &account,
                             QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    for (const FilePtr & file : qAsConst(files)) {
        d->filesIDs << file->id();
    }
}

FileDeleteJob::~FileDeleteJob()
{
    delete d;
}

void FileDeleteJob::start()
{
    if (d->filesIDs.isEmpty()) {
        emitResult();
        return;
    }

    const QString fileId = d->filesIDs.takeFirst();
    const QUrl url = DriveService::deleteFileUrl(fileId);

    QNetworkRequest request(url);
    enqueueRequest(request);
}


