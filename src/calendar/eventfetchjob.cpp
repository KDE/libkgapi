/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "eventfetchjob.h"
#include "account.h"
#include "calendarservice.h"
#include "debug.h"
#include "event.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN EventFetchJob::Private
{
public:
    QString calendarId;
    QString eventId;
    QString filter;
    QString syncToken;
    bool fetchDeleted = true;
    quint64 updatedTimestamp = 0;
    quint64 timeMin = 0;
    quint64 timeMax = 0;
};

EventFetchJob::EventFetchJob(const QString &calendarId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
    d->calendarId = calendarId;
}

EventFetchJob::EventFetchJob(const QString &eventId, const QString &calendarId, const AccountPtr &account, QObject *parent)
    : FetchJob(account, parent)
    , d(new Private)
{
    d->calendarId = calendarId;
    d->eventId = eventId;
}

EventFetchJob::~EventFetchJob() = default;

void EventFetchJob::setFetchDeleted(bool fetchDeleted)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify fetchDeleted property when job is running";
        return;
    }

    d->fetchDeleted = fetchDeleted;
}

bool EventFetchJob::fetchDeleted()
{
    return d->fetchDeleted;
}

void EventFetchJob::setFetchOnlyUpdated(quint64 timestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify setFetchOnlyUpdated property when job is running";
        return;
    }

    d->updatedTimestamp = timestamp;
}

quint64 EventFetchJob::fetchOnlyUpdated() const
{
    return d->updatedTimestamp;
}

void EventFetchJob::setTimeMax(quint64 timestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify timeMax property when job is running";
        return;
    }

    d->timeMax = timestamp;
}

quint64 EventFetchJob::timeMax() const
{
    return d->timeMax;
}

void EventFetchJob::setSyncToken(const QString &syncToken)
{
    d->syncToken = syncToken;
}

QString EventFetchJob::syncToken() const
{
    return d->syncToken;
}

void EventFetchJob::setTimeMin(quint64 timestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify timeMin property when job is running";
        return;
    }

    d->timeMin = timestamp;
}

quint64 EventFetchJob::timeMin() const
{
    return d->timeMin;
}

void EventFetchJob::setFilter(const QString &query)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify filter property when job is running";
        return;
    }

    d->filter = query;
}

QString EventFetchJob::filter() const
{
    return d->filter;
}

void EventFetchJob::start()
{
    QUrl url;
    if (d->eventId.isEmpty()) {
        url = CalendarService::fetchEventsUrl(d->calendarId);
        QUrlQuery query(url);
        query.addQueryItem(QStringLiteral("showDeleted"), Utils::bool2Str(d->fetchDeleted));
        if (!d->filter.isEmpty()) {
            query.addQueryItem(QStringLiteral("q"), d->filter);
        }
        if (d->syncToken.isEmpty()) {
            if (d->updatedTimestamp > 0) {
                query.addQueryItem(QStringLiteral("updatedMin"), Utils::ts2Str(d->updatedTimestamp));
            }
            if (d->timeMin > 0) {
                query.addQueryItem(QStringLiteral("timeMin"), Utils::ts2Str(d->timeMin));
            }
            if (d->timeMax > 0) {
                query.addQueryItem(QStringLiteral("timeMax"), Utils::ts2Str(d->timeMax));
            }
        } else {
            query.addQueryItem(QStringLiteral("syncToken"), d->syncToken);
        }
        url.setQuery(query);
    } else {
        url = CalendarService::fetchEventUrl(d->calendarId, d->eventId);
    }
    const QNetworkRequest request = CalendarService::prepareRequest(url);
    enqueueRequest(request);
}

ObjectsList EventFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
{
    if (reply->error() == QNetworkReply::ContentGoneError || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == Gone) {
        // Full sync required by server, redo request with no updatedMin and no syncToken
        d->updatedTimestamp = 0;
        d->syncToken.clear();
        start();
        // Errors are not cleared on success
        // Do it here or else the job will fail
        setError(KGAPI2::NoError);
        setErrorString(QString());
        return ObjectsList();
    }

    FeedData feedData;
    feedData.requestUrl = reply->url();
    ObjectsList items;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->eventId.isEmpty()) {
            items = CalendarService::parseEventJSONFeed(rawData, feedData);
        } else {
            items << CalendarService::JSONToEvent(rawData).dynamicCast<Object>();
        }
        d->syncToken = feedData.syncToken;
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const auto request = CalendarService::prepareRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}
