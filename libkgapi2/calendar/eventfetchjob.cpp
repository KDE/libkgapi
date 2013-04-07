/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "eventfetchjob.h"
#include "calendarservice.h"
#include "account.h"
#include "debug.h"
#include "event.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class EventFetchJob::Private
{
  public:
    Private(EventFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    QString calendarId;
    QString eventId;
    QString filter;
    bool fetchDeleted;
    quint64 updatedTimestamp;
    quint64 timeMin;
    quint64 timeMax;

  private:
    EventFetchJob * const q;
};

EventFetchJob::Private::Private(EventFetchJob* parent):
    fetchDeleted(true),
    updatedTimestamp(0),
    timeMin(0),
    timeMax(0),
    q(parent)
{
}

QNetworkRequest EventFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    request.setUrl(url);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    return request;
}

EventFetchJob::EventFetchJob(const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->calendarId = calendarId;
}

EventFetchJob::EventFetchJob(const QString& eventId, const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->calendarId = calendarId;
    d->eventId = eventId;
}

EventFetchJob::~EventFetchJob()
{
    delete d;
}

void EventFetchJob::setFetchDeleted(bool fetchDeleted)
{
    if (isRunning()) {
        kWarning() << "Can't modify fetchDeleted property when job is running";
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
        kWarning() << "Can't modify setFetchOnlyUpdated property when job is running";
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
        kWarning() << "Can't modify timeMax property when job is running";
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
        kWarning() << "Can't modify timeMin property when job is running";
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
        kWarning() << "Can't modify filter property when job is running";
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
        url.addQueryItem(QLatin1String("showDeleted"), Utils::bool2Str(d->fetchDeleted));
        if (!d->filter.isEmpty()) {
            url.addQueryItem(QLatin1String("q"), d->filter);
        }
        if (d->updatedTimestamp > 0) {
            url.addQueryItem(QLatin1String("updatedMin"), Utils::ts2Str(d->updatedTimestamp));
        }
        if (d->timeMin > 0) {
            url.addQueryItem(QLatin1String("timeMin"), Utils::ts2Str(d->timeMin));
        }
        if (d->timeMax > 0) {
            url.addQueryItem(QLatin1String("timeMax"), Utils::ts2Str(d->timeMax));
        }
    } else {
        url = CalendarService::fetchEventUrl(d->calendarId, d->eventId);
    }
    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList EventFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;

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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "eventfetchjob.moc"
