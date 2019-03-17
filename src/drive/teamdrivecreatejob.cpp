/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2019  David Barchiesi <david@barchie.si>
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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "teamdrivecreatejob.h"
#include "account.h"
#include "driveservice.h"
#include "teamdrive.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


namespace {
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

TeamdriveCreateJob::Private::Private(TeamdriveCreateJob *parent):
    q(parent)
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

    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    QUrlQuery query(url);
    if (!requestId.isEmpty()) {
        query.addQueryItem(RequestIdParam, requestId);
    }
    url.setQuery(query);
    request.setUrl(url);

    const QByteArray rawData = Teamdrive::toJSON(teamdrive);
    q->enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

TeamdriveCreateJob::TeamdriveCreateJob(const QString &requestId,
                                         const TeamdrivePtr &teamdrive,
                                         const AccountPtr &account,
                                         QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
{
    d->requestId = requestId;
    d->teamdrives << teamdrive;
}

TeamdriveCreateJob::TeamdriveCreateJob(const QString &requestId,
                                         const TeamdrivesList &teamdrives,
                                         const AccountPtr &account,
                                         QObject *parent):
    CreateJob(account, parent),
    d(new Private(this))
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

ObjectsList TeamdriveCreateJob::handleReplyWithItems(const QNetworkReply *reply,
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


