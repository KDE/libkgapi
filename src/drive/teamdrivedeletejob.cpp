/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "teamdrivedeletejob.h"
#include "account.h"
#include "driveservice.h"
#include "teamdrive.h"

#include <QNetworkRequest>

using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN TeamdriveDeleteJob::Private
{
public:
    QStringList teamdrivesIds;
};

TeamdriveDeleteJob::TeamdriveDeleteJob(const QString &teamdriveId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->teamdrivesIds << teamdriveId;
}

TeamdriveDeleteJob::TeamdriveDeleteJob(const QStringList &teamdrivesIds, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->teamdrivesIds << teamdrivesIds;
}

TeamdriveDeleteJob::TeamdriveDeleteJob(const TeamdrivePtr &teamdrive, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    d->teamdrivesIds << teamdrive->id();
}

TeamdriveDeleteJob::TeamdriveDeleteJob(const TeamdrivesList &teamdrives, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private)
{
    for (const TeamdrivePtr &teamdrive : std::as_const(teamdrives)) {
        d->teamdrivesIds << teamdrive->id();
    }
}

TeamdriveDeleteJob::~TeamdriveDeleteJob() = default;

void TeamdriveDeleteJob::start()
{
    if (d->teamdrivesIds.isEmpty()) {
        emitFinished();
        return;
    }

    const QString teamdriveId = d->teamdrivesIds.takeFirst();
    const QUrl url = DriveService::fetchTeamdriveUrl(teamdriveId);

    QNetworkRequest request(url);
    enqueueRequest(request);
}
