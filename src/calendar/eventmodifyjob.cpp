/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "eventmodifyjob.h"
#include "calendarservice.h"
#include "account.h"
#include "../debug.h"
#include "event.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN EventModifyJob::Private
{
  public:
    QueueHelper<EventPtr> events;
    QString calendarId;
    SendUpdatesPolicy updatesPolicy = SendUpdatesPolicy::All;
};

EventModifyJob::EventModifyJob(const EventPtr& event, const QString& calendarId, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->events << event;
    d->calendarId = calendarId;
}

EventModifyJob::EventModifyJob(const EventsList& events, const QString& calendarId, const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->events = events;
    d->calendarId = calendarId;
}

EventModifyJob::~EventModifyJob() = default;

void EventModifyJob::setSendUpdates(KGAPI2::SendUpdatesPolicy updatesPolicy)
{
    if (d->updatesPolicy != updatesPolicy) {
        d->updatesPolicy = updatesPolicy;
        Q_EMIT sendUpdatesChanged(d->updatesPolicy);
    }
}

SendUpdatesPolicy EventModifyJob::sendUpdates() const
{
    return d->updatesPolicy;
}

void EventModifyJob::start()
{
    if (d->events.atEnd()) {
        emitResult();
        return;
    }

    const EventPtr event = d->events.current();
    const auto request = CalendarService::prepareRequest(CalendarService::updateEventUrl(d->calendarId, event->id(), d->updatesPolicy));
    const QByteArray rawData = CalendarService::eventToJSON(event);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList EventModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitResult();
        return items;
    }

    items << CalendarService::JSONToEvent(rawData).dynamicCast<Event>();
    d->events.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}


