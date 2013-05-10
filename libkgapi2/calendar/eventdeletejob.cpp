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

#include "eventdeletejob.h"
#include "calendarservice.h"
#include "account.h"
#include "debug.h"
#include "event.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class EventDeleteJob::Private
{
  public:
    QueueHelper<QString> eventsIds;
    QString calendarId;
};

EventDeleteJob::EventDeleteJob(const EventPtr& event, const QString &calendarId, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    d->eventsIds << event->uid();
    d->calendarId = calendarId;
}

EventDeleteJob::EventDeleteJob(const EventsList& events, const QString& calendarId, const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private)
{
    Q_FOREACH(const EventPtr &event, events) {
        d->eventsIds << event->uid();
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
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    enqueueRequest(request);
}

void EventDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->eventsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}


#include "eventdeletejob.moc"
