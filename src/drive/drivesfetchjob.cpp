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

#include "drivesfetchjob.h"
#include "account.h"
#include "drives.h"
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

class Q_DECL_HIDDEN DrivesFetchJob::Private
{
  public:
    Private(DrivesFetchJob *parent);

    DrivesSearchQuery searchQuery;
    QString drivesId;

    int maxResults = 0;
    QVariant useDomainAdminAccess;

    QStringList fields;

  private:
    DrivesFetchJob *const q;
};

DrivesFetchJob::Private::Private(DrivesFetchJob *parent):
    q(parent)
{
}

DrivesFetchJob::DrivesFetchJob(const QString &drivesId,
                               const AccountPtr &account,
                               QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->drivesId = drivesId;
}

DrivesFetchJob::DrivesFetchJob(const DrivesSearchQuery &query,
                           const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->searchQuery = query;
}

DrivesFetchJob::DrivesFetchJob(const AccountPtr &account, QObject *parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

DrivesFetchJob::~DrivesFetchJob() = default;

void DrivesFetchJob::setMaxResults(int maxResults)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify maxResults property when job is running";
        return;
    }

    d->maxResults = maxResults;
}

int DrivesFetchJob::maxResults() const
{
    return d->maxResults;
}

void DrivesFetchJob::setUseDomainAdminAccess(bool useDomainAdminAccess)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify useDomainAdminAccess property when job is running";
        return;
    }

    d->useDomainAdminAccess = useDomainAdminAccess;
}

bool DrivesFetchJob::useDomainAdminAccess() const
{
    return d->useDomainAdminAccess.toBool();
}

void DrivesFetchJob::setFields(const QStringList &fields)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called setFields() on running job. Ignoring.";
        return;
    }

    d->fields = fields;
}

QStringList DrivesFetchJob::fields() const
{
    return d->fields;
}

void DrivesFetchJob::start()
{
    QUrl url;
    if (d->drivesId.isEmpty()) {
        url = DriveService::fetchDrivesUrl();
        applyRequestParameters(url);
    } else {
        url = DriveService::fetchDrivesUrl(d->drivesId);
        if (!d->fields.isEmpty()) {
            // Deserializing requires kind attribute, always force add it
            if (!d->fields.contains(Drives::Fields::Kind)) {
                d->fields << Drives::Fields::Kind;
            }
            Job::setFields(d->fields);
        }
    }

    QNetworkRequest request(url);
    enqueueRequest(request);
}


ObjectsList DrivesFetchJob::handleReplyWithItems(const QNetworkReply *reply,
        const QByteArray &rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();

    ObjectsList items;
    QString itemId;

    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->drivesId.isEmpty()) {
            items << Drives::fromJSONFeed(rawData, feedData);
        } else {
            items << Drives::fromJSON(rawData);
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


void DrivesFetchJob::applyRequestParameters(QUrl &url) {
    QUrlQuery query(url);
    if (d->maxResults != 0) {
        query.addQueryItem(MaxResultsAttr, QString::number(d->maxResults));
    }
    if (!d->useDomainAdminAccess.isNull()) {
        query.addQueryItem(UseDomainAdminAccessAttr, d->useDomainAdminAccess.toBool() ? True : False);
    }
    if (!d->searchQuery.isEmpty()) {
        query.addQueryItem(QStringLiteral("q"), d->searchQuery.serialize());
    }
    if (!d->fields.isEmpty()) {
        // Deserializing requires kind attribute, always force add it
        if (!d->fields.contains(Drives::Fields::Kind)) {
            d->fields << Drives::Fields::Kind;
        }
        QString itemFields = Job::buildSubfields(Drives::Fields::Items, d->fields);
        Job::setFields({
            Drives::Fields::Kind,
            Drives::Fields::NextPageToken,
            itemFields
        });
    }
    url.setQuery(query);
}
