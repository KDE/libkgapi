/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "teamdrivemodifyjob.h"
#include "account.h"
#include "driveservice.h"
#include "teamdrive.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN TeamdriveModifyJob::Private
{
  public:
    Private(TeamdriveModifyJob *parent);
    void processNext();

    TeamdrivesList teamdrives;

  private:
    TeamdriveModifyJob *const q;
};

TeamdriveModifyJob::Private::Private(TeamdriveModifyJob *parent):
    q(parent)
{
}

void TeamdriveModifyJob::Private::processNext()
{
    if (teamdrives.isEmpty()) {
        q->emitFinished();
        return;
    }

    const TeamdrivePtr teamdrive = teamdrives.takeFirst();
    const QUrl url = DriveService::fetchTeamdriveUrl(teamdrive->id());

    QNetworkRequest request(url);

    const QByteArray rawData = Teamdrive::toJSON(teamdrive);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

TeamdriveModifyJob::TeamdriveModifyJob(const TeamdrivePtr &teamdrive,
                                         const AccountPtr &account,
                                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->teamdrives << teamdrive;
}

TeamdriveModifyJob::TeamdriveModifyJob(const TeamdrivesList &teamdrives,
                                         const AccountPtr &account,
                                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->teamdrives << teamdrives;
}

TeamdriveModifyJob::~TeamdriveModifyJob() = default;

void TeamdriveModifyJob::start()
{
    d->processNext();
}

ObjectsList TeamdriveModifyJob::handleReplyWithItems(const QNetworkReply *reply,
                                                        const QByteArray &rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << Teamdrive::fromJSON(rawData);
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    // Enqueue next item or finish
    d->processNext();

    return items;
}


