/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "calendardeletejob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "../debug.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN CalendarDeleteJob::Private
{
  public:
    QueueHelper<QString> calendarsIds;
};

CalendarDeleteJob::CalendarDeleteJob(const CalendarPtr& calendar, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds.enqueue(calendar->uid());
}

CalendarDeleteJob::CalendarDeleteJob(const CalendarsList& calendars, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    for (const CalendarPtr &calendar : calendars) {
        d->calendarsIds.enqueue(calendar->uid());
    }
}

CalendarDeleteJob::CalendarDeleteJob(const QString &calendarId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds.enqueue(calendarId);
}

CalendarDeleteJob::CalendarDeleteJob(const QStringList &calendarsIds, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds = calendarsIds;
}


CalendarDeleteJob::~CalendarDeleteJob() = default;

void CalendarDeleteJob::start()
{
    if (d->calendarsIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString calendarId = d->calendarsIds.current();
    const auto request = CalendarService::prepareRequest(CalendarService::removeCalendarUrl(calendarId));

    enqueueRequest(request);
}

void CalendarDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->calendarsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}



