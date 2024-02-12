/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "personfetchjob.h"
#include "peopleservice.h"
#include "person.h"
#include "../debug.h"
#include "types.h"
#include "utils.h"
#include "account.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN PersonFetchJob::Private
{
public:
    explicit Private(PersonFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);
    ObjectsList processReceivedData(const QByteArray &rawData);

    QString personResourceName;
    QString syncToken;
    QString receivedSyncToken;

public Q_SLOTS:
    void startFetch();

private:
    PersonFetchJob * const q;
};

PersonFetchJob::Private::Private(PersonFetchJob *parent)
    : q(parent)
{
}

QNetworkRequest PersonFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Host", "people.googleapis.com");
    return request;
}

void PersonFetchJob::Private::startFetch()
{
    QUrl url;
    if (personResourceName.isEmpty()) {
        url = PeopleService::fetchAllContactsUrl(syncToken);
    } else {
        url = PeopleService::fetchContactUrl(personResourceName);
    }

    const QNetworkRequest request = createRequest(url);
    q->enqueueRequest(request);
}

ObjectsList PersonFetchJob::Private::processReceivedData(const QByteArray &rawData)
{
    FeedData feedData;
    ObjectsList items;

    if (personResourceName.isEmpty()) {
        items = PeopleService::parseConnectionsJSONFeed(feedData, rawData, syncToken);
    } else {
        const auto jsonDocumentFromData = QJsonDocument::fromJson(rawData);
        if(jsonDocumentFromData.isObject()) {
            items << People::Person::fromJSON(jsonDocumentFromData.object());
        } else {
            qCDebug(KGAPIDebug) << "JSON document does not have object";
        }
    }

    if (feedData.nextPageUrl.isValid()) {
        q->emitProgress(feedData.startIndex, feedData.totalResults);

        const QNetworkRequest request = createRequest(feedData.nextPageUrl);
        q->enqueueRequest(request);
    } else {
        receivedSyncToken = feedData.syncToken;
        q->emitFinished();
    }

    return items;
}

PersonFetchJob::PersonFetchJob(const AccountPtr& account, QObject* parent)
    : FetchJob(account, parent)
    , d(std::make_unique<Private>(this))
{
}

PersonFetchJob::PersonFetchJob(const QString &resourceName, const AccountPtr &account, QObject* parent)
    : FetchJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->personResourceName = resourceName;
}

PersonFetchJob::~PersonFetchJob() = default;

QString PersonFetchJob::syncToken() const
{
    return d->syncToken;
}

void PersonFetchJob::setSyncToken(const QString &syncToken)
{
    if (syncToken == d->syncToken) {
        return;
    }

    d->syncToken = syncToken;
    Q_EMIT syncTokenChanged();
}

QString PersonFetchJob::receivedSyncToken() const
{
    return d->receivedSyncToken;
}

void PersonFetchJob::start()
{
    d->startFetch();
}

bool PersonFetchJob::handleError(int statusCode, const QByteArray &rawData)
{
    if (statusCode == KGAPI2::BadRequest) {
        const auto error = QJsonDocument::fromJson(rawData);
        if (error[u"error"][u"status"].toString() == u"INVALID_ARGUMENT") {
            qCDebug(KGAPIDebug) << "Sync token is invalid, redoing request with no syncToken";
            d->syncToken.clear();
            d->startFetch();
            return true;
        }
        for (const auto detail : error[u"error"][u"details"].toArray()) {
            if (detail[u"reason"].toString() == u"EXPIRED_SYNC_TOKEN") {
                qCDebug(KGAPIDebug) << "Sync token expired, redoing request with no syncToken";
                d->syncToken.clear();
                d->startFetch();
                return true;
            }
        }
    }

    return FetchJob::handleError(statusCode, rawData);
}

ObjectsList PersonFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    const auto contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    const auto ct = Utils::stringToContentType(contentType);

    if (ct == KGAPI2::JSON) {
        return d->processReceivedData(rawData);
    }

    return ObjectsList();
}

}

#include "moc_personfetchjob.cpp"
