/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "teamdrivefetchjob.h"
#include "account.h"
#include "teamdrive.h"
#include "debug.h"
#include "driveservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

namespace {
    static const QString MaxResultsAttr = QStringLiteral("maxResults");
    static const QString UseDomainAdminAccessAttr = QStringLiteral("useDomainAdminAccess");
}
using namespace KGAPI2;
using namespace KGAPI2::Drive;

class Q_DECL_HIDDEN TeamdriveFetchJob::Private
{
  public:
    Private(TeamdriveFetchJob *parent);

    TeamdriveSearchQuery searchQuery;
    QString teamdriveId;

    int maxResults = 0;
    bool useDomainAdminAccess = false;

    QStringList fields;

  private:
    TeamdriveFetchJob *const q;
};

TeamdriveFetchJob::Private::Private(TeamdriveFetchJob *parent):
    q(parent)
{
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

void TeamdriveFetchJob::setFields(const QStringList &fields)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called setFields() on running job. Ignoring.";
        return;
    }

    d->fields = fields;
}

QStringList TeamdriveFetchJob::fields() const
{
    return d->fields;
}

void TeamdriveFetchJob::start()
{
    QUrl url;
    if (d->teamdriveId.isEmpty()) {
        url = DriveService::fetchTeamdrivesUrl();
        applyRequestParameters(url);
    } else {
        url = DriveService::fetchTeamdriveUrl(d->teamdriveId);
        if (!d->fields.isEmpty()) {
            // Deserializing requires kind attribute, always force add it
            if (!d->fields.contains(Teamdrive::Fields::Kind)) {
                d->fields << Teamdrive::Fields::Kind;
            }
            Job::setFields(d->fields);
        }
    }

    QNetworkRequest request(url);
    enqueueRequest(request);
}


ObjectsList TeamdriveFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();

    ObjectsList items;

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
        QNetworkRequest request(feedData.nextPageUrl);
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
        query.addQueryItem(UseDomainAdminAccessAttr, Utils::bool2Str(d->useDomainAdminAccess));
    }
    if (!d->searchQuery.isEmpty()) {
        query.addQueryItem(QStringLiteral("q"), d->searchQuery.serialize());
    }
    if (!d->fields.isEmpty()) {
        // Deserializing requires kind attribute, always force add it
        if (!d->fields.contains(Teamdrive::Fields::Kind)) {
            d->fields << Teamdrive::Fields::Kind;
        }
        QString itemFields = Job::buildSubfields(Teamdrive::Fields::Items, d->fields);
        Job::setFields({
            Teamdrive::Fields::Kind,
            Teamdrive::Fields::NextPageToken,
            itemFields
        });
    }
    url.setQuery(query);
}
