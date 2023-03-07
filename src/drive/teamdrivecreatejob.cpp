/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "teamdrivecreatejob.h"
#include "driveservice.h"
#include "teamdrive.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace
{
static const QString RequestIdParam = QStringLiteral("requestId");
}
using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN TeamdriveCreateJob::Private
{
public:
    Private(TeamdriveCreateJob *parent);
    void processNext();

    TeamdrivesList teamdrives;
    QString requestId;

private:
    TeamdriveCreateJob *const q;
};

TeamdriveCreateJob::Private::Private(TeamdriveCreateJob *parent)
    : q(parent)
{
}

void TeamdriveCreateJob::Private::processNext()
{
    if (teamdrives.isEmpty()) {
        q->emitFinished();
        return;
    }

    const TeamdrivePtr teamdrive = teamdrives.takeFirst();

    QUrl url = DriveService::fetchTeamdrivesUrl();

    QUrlQuery query(url);
    if (!requestId.isEmpty()) {
        query.addQueryItem(RequestIdParam, requestId);
    }
    url.setQuery(query);

    QNetworkRequest request(url);

    const QByteArray rawData = Teamdrive::toJSON(teamdrive);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

TeamdriveCreateJob::TeamdriveCreateJob(const QString &requestId, const TeamdrivePtr &teamdrive, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->requestId = requestId;
    d->teamdrives << teamdrive;
}

TeamdriveCreateJob::TeamdriveCreateJob(const QString &requestId, const TeamdrivesList &teamdrives, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private(this))
{
    d->requestId = requestId;
    d->teamdrives = teamdrives;
}

TeamdriveCreateJob::~TeamdriveCreateJob() = default;

QString TeamdriveCreateJob::requestId() const
{
    return d->requestId;
}

void TeamdriveCreateJob::start()
{
    d->processNext();
}

ObjectsList TeamdriveCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
