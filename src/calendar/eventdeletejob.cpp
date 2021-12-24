/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "eventdeletejob.h"
#include "calendarservice.h"
#include "account.h"
#include "debug.h"
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


EventDeleteJob::~EventDeleteJob() = default;

void EventDeleteJob::start()
{
    if (d->eventsIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString eventId = d->eventsIds.current();
    const auto request = CalendarService::prepareRequest(CalendarService::removeEventUrl(d->calendarId, eventId));

    enqueueRequest(request);
}

void EventDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->eventsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}



