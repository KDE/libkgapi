/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "fileresumablecreatejob.h"
#include "driveservice.h"

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN FileResumableCreateJob::Private
{

};

FileResumableCreateJob::FileResumableCreateJob(const AccountPtr &account,
                             QObject *parent):
    FileAbstractResumableJob(account, parent),
    d(new Private)
{
}

FileResumableCreateJob::FileResumableCreateJob(const FilePtr &metadata,
                             const AccountPtr &account,
                             QObject *parent):
    FileAbstractResumableJob(metadata, account, parent),
    d(new Private)
{
}

FileResumableCreateJob::~FileResumableCreateJob() = default;

QUrl FileResumableCreateJob::createUrl()
{
    return DriveService::uploadMediaFileUrl();
}
