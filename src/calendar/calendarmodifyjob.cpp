/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "calendarmodifyjob.h"
#include "calendar.h"
#include "calendarservice.h"
#include "private/queuehelper_p.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN CalendarModifyJob::Private
{
public:
    QueueHelper<CalendarPtr> calendars;
};

CalendarModifyJob::CalendarModifyJob(const CalendarPtr &calendar, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private)
{
    d->calendars << calendar;
}

CalendarModifyJob::CalendarModifyJob(const CalendarsList &calendars, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private)
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

ObjectsList CalendarModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
