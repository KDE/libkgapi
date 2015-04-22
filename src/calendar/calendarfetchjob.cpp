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


#include "calendarfetchjob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "../debug.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


using namespace KGAPI2;

class CalendarFetchJob::Private
{
  public:
    Private(CalendarFetchJob *parent);

    QNetworkRequest createRequest(const QUrl &url);

    QString calendarId;

  private:
    CalendarFetchJob * const q;
};

CalendarFetchJob::Private::Private(CalendarFetchJob* parent):
    q(parent)
{
}

QNetworkRequest CalendarFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    request.setUrl(url);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    return request;
}

CalendarFetchJob::CalendarFetchJob(const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
}

CalendarFetchJob::CalendarFetchJob(const QString& calendarId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->calendarId = calendarId;
}

CalendarFetchJob::~CalendarFetchJob()
{
    delete d;
}

void CalendarFetchJob::start()
{
    QUrl url;
    if (d->calendarId.isEmpty()) {
        url = CalendarService::fetchCalendarsUrl();
    } else {
        url = CalendarService::fetchCalendarUrl(d->calendarId);
    }
    qDebug() << "CalendarFetchJob::url()" << CalendarService::fetchCalendarsUrl();
    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList CalendarFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->calendarId.isEmpty()) {
            items =  CalendarService::parseCalendarJSONFeed(rawData, feedData);
        } else {
            items << CalendarService::JSONToCalendar(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


