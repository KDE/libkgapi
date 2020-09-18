/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "eventcreatejob.h"
#include "calendarservice.h"
#include "account.h"
#include "../debug.h"
#include "event.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN EventCreateJob::Private
{
  public:
    QueueHelper<EventPtr> events;
    QString calendarId;
    SendUpdatesPolicy updatesPolicy = SendUpdatesPolicy::All;
};

EventCreateJob::EventCreateJob(const EventPtr& event, const QString &calendarId, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->events.enqueue(event);
    d->calendarId = calendarId;
}

EventCreateJob::EventCreateJob(const EventsList& events, const QString& calendarId, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->events = events;
    d->calendarId = calendarId;
}

EventCreateJob::~EventCreateJob() = default;

void EventCreateJob::setSendUpdates(SendUpdatesPolicy policy)
{
    if (d->updatesPolicy != policy) {
        d->updatesPolicy = policy;
        Q_EMIT sendUpdatesChanged(d->updatesPolicy);
    }
}

SendUpdatesPolicy EventCreateJob::sendUpdates() const
{
    return d->updatesPolicy;
}

void EventCreateJob::start()
{
    if (d->events.atEnd()) {
        emitFinished();
        return;
    }

    const EventPtr event = d->events.current();
    const auto request = CalendarService::prepareRequest(CalendarService::createEventUrl(d->calendarId, d->updatesPolicy));
    const QByteArray rawData = CalendarService::eventToJSON(event, CalendarService::EventSerializeFlag::NoID);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList EventCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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

    items << CalendarService::JSONToEvent(rawData).dynamicCast<Object>();
    d->events.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}


