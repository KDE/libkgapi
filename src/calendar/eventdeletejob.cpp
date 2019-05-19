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

#include "eventdeletejob.h"
#include "calendarservice.h"
#include "account.h"
#include "../debug.h"
#include "event.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN EventDeleteJob::Private
{
  public:
    QueueHelper<QString> eventsIds;
    QString calendarId;
};

EventDeleteJob::EventDeleteJob(const EventPtr& event, const QString &calendarId, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->eventsIds << event->id();
    d->calendarId = calendarId;
}

EventDeleteJob::EventDeleteJob(const EventsList& events, const QString& calendarId, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    for (const EventPtr &event : events) {
        d->eventsIds << event->id();
    }
    d->calendarId = calendarId;
}

EventDeleteJob::EventDeleteJob(const QString &eventId, const QString &calendarId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->eventsIds << eventId;
    d->calendarId = calendarId;
}

EventDeleteJob::EventDeleteJob(const QStringList &eventIds, const QString &calendarId, const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->eventsIds = eventIds;
    d->calendarId = calendarId;
}


EventDeleteJob::~EventDeleteJob()
{
    delete d;
}

void EventDeleteJob::start()
{
    if (d->eventsIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString eventId = d->eventsIds.current();
    const QUrl url = CalendarService::removeEventUrl(d->calendarId, eventId);
    QNetworkRequest request(url);
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request);
}

void EventDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->eventsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}



