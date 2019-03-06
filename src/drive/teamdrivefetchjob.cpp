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

#include "teamdrivefetchjob.h"
#include "account.h"
#include "teamdrive.h"
#include "../debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace {
    static const QString MaxResultsAttr = QStringLiteral("maxResults");
    static const QString UseDomainAdminAccessAttr = QStringLiteral("useDomainAdminAccess");
    static const QString True = QStringLiteral("true");
    static const QString False = QStringLiteral("false");
}
using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN TeamdriveFetchJob::Private
{
  public:
    Private(TeamdriveFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    TeamdriveSearchQuery searchQuery;
    QString teamdriveId;

    int maxResults = 0;
    bool useDomainAdminAccess = false;

  private:
    TeamdriveFetchJob *const q;
};

TeamdriveFetchJob::Private::Private(TeamdriveFetchJob *parent):
    q(parent)
{
}

QNetworkRequest TeamdriveFetchJob::Private::createRequest(const QUrl &url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    return request;
}


TeamdriveFetchJob::TeamdriveFetchJob(const QString &teamdriveId,
                               const AccountPtr &account,
                               QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->teamdriveId = teamdriveId;
}

TeamdriveFetchJob::TeamdriveFetchJob(const TeamdriveSearchQuery &query,
                           const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->useDomainAdminAccess = true;
    d->searchQuery = query;
}

TeamdriveFetchJob::TeamdriveFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

TeamdriveFetchJob::~TeamdriveFetchJob() = default;

void TeamdriveFetchJob::setMaxResults(int maxResults)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property when job is running";
        return;
    }

    d->maxResults = maxResults;
}

int TeamdriveFetchJob::maxResults() const
{
    return d->maxResults;
}

void TeamdriveFetchJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify useDomainAdminAccess property when job is running";
        return;
    }

    d->useDomainAdminAccess = useDomainAdminAccess;
}

bool TeamdriveFetchJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess;
}

void TeamdriveFetchJob::start()
{
    QUrl url;
    if (d->teamdriveId.isEmpty()) {
        url = DriveService::fetchTeamdrivesUrl();
        applyRequestParameters(url);
    } else {
        url = DriveService::fetchTeamdriveUrl(d->teamdriveId);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}


ObjectsList TeamdriveFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->teamdriveId.isEmpty()) {
            items << Teamdrive::fromJSONFeed(rawData, feedData);
        } else {
            items << Teamdrive::fromJSON(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        // Reapply query options
        applyRequestParameters(feedData.nextPageUrl);
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


void TeamdriveFetchJob::applyRequestParameters(QUrl &url) {
    QUrlQuery query(url);
    if (d->maxResults != 0) {
        query.addQueryItem(MaxResultsAttr, QString::number(d->maxResults));
    }
    if (d->useDomainAdminAccess != false) {
        query.addQueryItem(UseDomainAdminAccessAttr, d->useDomainAdminAccess ? True : False);
    }
    if (!d->searchQuery.isEmpty()) {
        query.addQueryItem(QStringLiteral("q"), d->searchQuery.serialize());
    }
    url.setQuery(query);
}
