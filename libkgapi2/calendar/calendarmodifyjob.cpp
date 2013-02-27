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

#include "calendarmodifyjob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "debug.h"
#include "utils.h"

#include <KDE/KLocalizedString>

#include <QtNetwork/QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;

class CalendarModifyJob::Private
{
  public:
    CalendarsList calendars;
};

CalendarModifyJob::CalendarModifyJob(const CalendarPtr& calendar, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->calendars << calendar;
}

CalendarModifyJob::CalendarModifyJob(const CalendarsList& calendars, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->calendars = calendars;
}

CalendarModifyJob::~CalendarModifyJob()
{
    delete d;
}

void CalendarModifyJob::start()
{
    if (d->calendars.isEmpty()) {
        emitFinished();
        return;
    }

    const CalendarPtr calendar = d->calendars.takeFirst();

    const QUrl url = CalendarService::updateCalendarUrl(calendar->uid());
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = CalendarService::calendarToJSON(calendar);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList CalendarModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct == KGAPI2::JSON) {
        items << CalendarService::JSONToCalendar(rawData).dynamicCast<Object>();
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
    }

    // Enqueue next item or finish
    start();

    return items;
}

#include "calendarmodifyjob.moc"