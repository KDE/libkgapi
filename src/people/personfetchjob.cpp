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
    Private(PersonFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);
    ObjectsList processReceivedData(const QByteArray &rawData);

    QString readMask;
    QString fetchQuery;
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
    if (fetchQuery.isEmpty()) {
        url = PeopleService::fetchAllContactsUrl(syncToken);
    } else {
        url = PeopleService::fetchContactUrl(fetchQuery, readMask);
    }

    const QNetworkRequest request = createRequest(url);
    q->enqueueRequest(request);
}

ObjectsList PersonFetchJob::Private::processReceivedData(const QByteArray &rawData)
{
    FeedData feedData;
    ObjectsList items;

    if (fetchQuery.isEmpty()) {
        items = PeopleService::parseConnectionsJSONFeed(feedData, rawData);
    } else {
        const auto jsonDocumentFromData = QJsonDocument::fromJson(rawData);
        if(jsonDocumentFromData.isObject()) {
            const auto results = jsonDocumentFromData.object().value(QStringLiteral("results")).toArray();

            for(const auto &result : results) {
                if(result.isObject()) {
                    items << People::Person::fromJSON(result.toObject());
                } else {
                    qDebug() << "Result was not an object... skipping.";
                }
            }

        } else {
            qDebug() << "JSON document does not have object";
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

PersonFetchJob::PersonFetchJob(const QString &fetchQuery,
                               const QString &readMask,
                               const AccountPtr &account,
                               QObject* parent)
    : FetchJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->fetchQuery = fetchQuery;
    d->readMask = readMask;
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
