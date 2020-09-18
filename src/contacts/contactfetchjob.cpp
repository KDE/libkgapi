/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "contactfetchjob.h"
#include "contact.h"
#include "contactsservice.h"
#include "../debug.h"
#include "utils.h"
#include "account.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN ContactFetchJob::Private
{
  public:
    Private(ContactFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);

    bool fetchDeleted = true;
    QString contactId;
    quint64 timestamp = 0;
    QString filter;

  private:
    ContactFetchJob * const q;
};

ContactFetchJob::Private::Private(ContactFetchJob *parent):
    q(parent)
{
}

QNetworkRequest ContactFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", ContactsService::APIVersion().toLatin1());

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    return request;
}

ContactFetchJob::ContactFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

ContactFetchJob::ContactFetchJob(const QString& contactId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->contactId = contactId;
}

ContactFetchJob::~ContactFetchJob()
{
    delete d;
}

bool ContactFetchJob::fetchDeleted() const
{
    return d->fetchDeleted;
}

void ContactFetchJob::setFetchDeleted(bool fetchDeleted)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify fetchDeleted property when job is running";
        return;
    }

    d->fetchDeleted = fetchDeleted;
}

quint64 ContactFetchJob::fetchOnlyUpdated()
{
    return d->timestamp;
}

void ContactFetchJob::setFetchOnlyUpdated(quint64 timestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify fetchOnlyUpdated property when job is running";
        return;
    }

    d->timestamp = timestamp;
}

QString ContactFetchJob::filter() const
{
    return d->filter;
}

void ContactFetchJob::setFilter(const QString &query)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify filter property when job is running";
        return;
    }

    d->filter = query;
}

void ContactFetchJob::start()
{
    QUrl url;
    if (d->contactId.isEmpty()) {
        url = ContactsService::fetchAllContactsUrl(account()->accountName(), d->fetchDeleted);
        QUrlQuery query(url);
        if (d->timestamp > 0) {
            query.addQueryItem(QStringLiteral("updated-min"), Utils::ts2Str(d->timestamp));
        }
        if (!d->filter.isEmpty()) {
            query.addQueryItem(QStringLiteral("q"), d->filter);
        }
        url.setQuery(query);
    } else {
        url = ContactsService::fetchContactUrl(account()->accountName(), d->contactId);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList ContactFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    FeedData feedData;
    ObjectsList items;
    QString itemId;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->contactId.isEmpty()) {
            items = ContactsService::parseJSONFeed(rawData, feedData);
        } else {
            items << ContactsService::JSONToContact(rawData);
        }

        if (feedData.nextPageUrl.isValid()) {
            emitPercent(feedData.startIndex, feedData.totalResults);

            const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
            enqueueRequest(request);
        } else {
            emitResult();
        }

        return items;
    }

    return ObjectsList();
}



