/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 *
 */

#include "contactgroupfetchjob.h"
#include "peopleservice.h"
#include "contactgroup.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace KGAPI2::People
{

class Q_DECL_HIDDEN ContactGroupFetchJob::Private
{
public:
    explicit Private(ContactGroupFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);

    QString resourceName;

private:
    ContactGroupFetchJob * const q;
};

ContactGroupFetchJob::Private::Private(ContactGroupFetchJob *parent)
    : q(parent)
{
}

ContactGroupFetchJob::ContactGroupFetchJob(const AccountPtr& account, QObject* parent)
    : FetchJob(account, parent)
    , d(std::make_unique<Private>(this))
{
}

QNetworkRequest ContactGroupFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Host", "people.googleapis.com");
    return request;
}

ContactGroupFetchJob::ContactGroupFetchJob(const QString &resourceName,
                                           const AccountPtr &account,
                                           QObject* parent)
    : FetchJob(account, parent)
    , d(std::make_unique<Private>(this))
{
    d->resourceName = resourceName;
}

ContactGroupFetchJob::~ContactGroupFetchJob() = default;

void ContactGroupFetchJob::start()
{
    QUrl url;
    if (d->resourceName.isEmpty()) {
        url = PeopleService::fetchAllContactGroupsUrl();
    } else {
        url = PeopleService::fetchContactGroupUrl(d->resourceName);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}


ObjectsList ContactGroupFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    ObjectsList items;

    const auto contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);

    if (ct != KGAPI2::JSON) {
        return ObjectsList();
    }

    if (d->resourceName.isEmpty()) {
        items = PeopleService::parseContactGroupsJSONFeed(feedData, rawData);
    } else {
        const auto jsonDocumentFromData = QJsonDocument::fromJson(rawData);
        if(jsonDocumentFromData.isObject()) {
            items << People::ContactGroup::fromJSON(jsonDocumentFromData.object());
        } else {
            qDebug() << "JSON document does not have object";
        }
    }

    if (feedData.nextPageUrl.isValid()) {
        emitProgress(feedData.startIndex, feedData.totalResults);

        const auto request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    } else {
        emitFinished();
    }

    return items;
}

}

#include "moc_contactgroupfetchjob.cpp"
