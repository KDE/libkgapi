/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "calendarcreatejob.h"
#include "calendarservice.h"
#include "object.h"
#include "account.h"
#include "calendar.h"
#include "../debug.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN CalendarCreateJob::Private
{
  public:
    QueueHelper<CalendarPtr> calendars;
};

CalendarCreateJob::CalendarCreateJob(const CalendarPtr& calendar, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->calendars.enqueue(calendar);
}

CalendarCreateJob::CalendarCreateJob(const CalendarsList& calendars, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->calendars = calendars;
}


CalendarCreateJob::~CalendarCreateJob() = default;

void CalendarCreateJob::start()
{
    if (d->calendars.atEnd()) {
        emitFinished();
        return;
    }

    const CalendarPtr calendar = d->calendars.current();
    const auto request = CalendarService::prepareRequest(CalendarService::createCalendarUrl());
    const QByteArray rawData = CalendarService::calendarToJSON(calendar);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList CalendarCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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
    start();

    return items;
}


