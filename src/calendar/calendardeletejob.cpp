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
    d->calendarsIds << calendar->uid();
}

CalendarDeleteJob::CalendarDeleteJob(const CalendarsList& calendars, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    for (const CalendarPtr &calendar : calendars) {
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
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request);
}

void CalendarDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->calendarsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}



