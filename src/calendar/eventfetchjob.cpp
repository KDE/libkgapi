/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
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

#include "eventfetchjob.h"
#include "calendarservice.h"
#include "account.h"
#include "../debug.h"
#include "event.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN EventFetchJob::Private
{
  public:
    QString calendarId;
    QString eventId;
    QString filter;
    bool fetchDeleted = true;
    quint64 updatedTimestamp = 0;
    quint64 timeMin = 0;
    quint64 timeMax = 0;
};

EventFetchJob::EventFetchJob(const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private)
{
    d->calendarId = calendarId;
}

EventFetchJob::EventFetchJob(const QString& eventId, const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private)
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

quint64 EventFetchJob::fetchOnlyUpdated()
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
        if (d->updatedTimestamp > 0) {
            query.addQueryItem(QStringLiteral("updatedMin"), Utils::ts2Str(d->updatedTimestamp));
        }
        if (d->timeMin > 0) {
            query.addQueryItem(QStringLiteral("timeMin"), Utils::ts2Str(d->timeMin));
        }
        if (d->timeMax > 0) {
            query.addQueryItem(QStringLiteral("timeMax"), Utils::ts2Str(d->timeMax));
        }
        url.setQuery(query);
    } else {
        url = CalendarService::fetchEventUrl(d->calendarId, d->eventId);
    }
    const QNetworkRequest request = CalendarService::prepareRequest(url);
    enqueueRequest(request);
}

ObjectsList EventFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();
    ObjectsList items;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->eventId.isEmpty()) {
            items =  CalendarService::parseEventJSONFeed(rawData, feedData);
        } else {
            items << CalendarService::JSONToEvent(rawData).dynamicCast<Object>();
        }
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


