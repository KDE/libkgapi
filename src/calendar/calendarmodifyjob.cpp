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

#include "calendarmodifyjob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "../debug.h"
#include "utils.h"
#include "private/queuehelper_p.h"


#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;

class Q_DECL_HIDDEN CalendarModifyJob::Private
{
  public:
    QueueHelper<CalendarPtr> calendars;
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

CalendarModifyJob::~CalendarModifyJob() = default;

void CalendarModifyJob::start()
{
    if (d->calendars.atEnd()) {
        emitFinished();
        return;
    }

    const CalendarPtr calendar = d->calendars.current();
    const auto request = CalendarService::prepareRequest(CalendarService::updateCalendarUrl(calendar->uid()));
    const QByteArray rawData = CalendarService::calendarToJSON(calendar);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList CalendarModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << CalendarService::JSONToCalendar(rawData).dynamicCast<Object>();
    d->calendars.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}


