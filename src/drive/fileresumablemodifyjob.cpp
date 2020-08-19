/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "fileresumablemodifyjob.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QUrlQuery>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileResumableModifyJob::Private
{
  public:
      QString fileId;
      bool createNewRevision = true;
      bool changeModifiedDate = false;
      bool updateViewedDate = true;
};

FileResumableModifyJob::FileResumableModifyJob(const FilePtr &metadata,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractResumableJob(metadata, account, parent),
    d(new Private)
{
    d->fileId = metadata->id();
}

FileResumableModifyJob::FileResumableModifyJob(const QString &fileId,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractResumableJob(account, parent),
    d(new Private)
{
    d->fileId = fileId;
}

FileResumableModifyJob::~FileResumableModifyJob() = default;

bool FileResumableModifyJob::createNewRevision() const
{
    return d->createNewRevision;
}

void FileResumableModifyJob::setCreateNewRevision(bool createNewRevision)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify createNewRevision property when the job is running";
        return;
    }

    d->createNewRevision = createNewRevision;
}

bool FileResumableModifyJob::updateModifiedDate() const
{
    return d->updateViewedDate;
}

void FileResumableModifyJob::setUpdateModifiedDate(bool updateModifiedDate)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify updateModifiedDate property when the job is running";
        return;
    }

    d->updateViewedDate = updateModifiedDate;
}

bool FileResumableModifyJob::updateViewedDate() const
{
    return d->updateViewedDate;
}

void FileResumableModifyJob::setUpdateViewedDate(bool updateViewedDate)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify updateViewedDate property when job is running";
        return;
    }

    d->updateViewedDate = updateViewedDate;
}


QUrl FileResumableModifyJob::createUrl()
{
    QUrl url = DriveService::uploadMediaFileUrl(d->fileId);

    QUrlQuery query(url);
    query.addQueryItem(QStringLiteral("newRevision"), Utils::bool2Str(d->createNewRevision));
    query.addQueryItem(QStringLiteral("setModifiedDate"), Utils::bool2Str(d->changeModifiedDate));
    query.addQueryItem(QStringLiteral("updateViewedDate"), Utils::bool2Str(d->updateViewedDate));
    url.setQuery(query);

    return url;
}
