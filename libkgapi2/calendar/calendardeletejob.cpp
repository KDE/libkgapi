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

#include "calendardeletejob.h"
#include "calendarservice.h"
#include "account.h"
#include "calendar.h"
#include "debug.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class CalendarDeleteJob::Private
{
  public:
    QueueHelper<QString> calendarsIds;
};

CalendarDeleteJob::CalendarDeleteJob(const CalendarPtr& calendar, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds << calendar->uid();
}

CalendarDeleteJob::CalendarDeleteJob(const CalendarsList& calendars, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    Q_FOREACH(const CalendarPtr &calendar, calendars) {
        d->calendarsIds << calendar->uid();
    }
}

CalendarDeleteJob::CalendarDeleteJob(const QString &calendarId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds << calendarId;
}

CalendarDeleteJob::CalendarDeleteJob(const QStringList &calendarsIds, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->calendarsIds = calendarsIds;
}


CalendarDeleteJob::~CalendarDeleteJob()
{
    delete d;
}

void CalendarDeleteJob::start()
{
    if (d->calendarsIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString calendarId = d->calendarsIds.current();
    const QUrl url = CalendarService::removeCalendarUrl(calendarId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    enqueueRequest(request);
}

void CalendarDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->calendarsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}


#include "calendardeletejob.moc"
