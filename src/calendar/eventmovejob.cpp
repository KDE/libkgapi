/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "eventmovejob.h"
#include "calendarservice.h"
#include "event.h"
#include "private/queuehelper_p.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN EventMoveJob::Private
{
public:
    QueueHelper<QString> eventsIds;
    QString source;
    QString destination;
};

EventMoveJob::EventMoveJob(const EventPtr &event,
                           const QString &sourceCalendarId,
                           const QString &destinationCalendarId,
                           const AccountPtr &account,
                           QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private())
{
    d->eventsIds << event->id();
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const EventsList &events,
                           const QString &sourceCalendarId,
                           const QString &destinationCalendarId,
                           const AccountPtr &account,
                           QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private())
{
    for (const EventPtr &event : events) {
        d->eventsIds << event->id();
    }
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const QString &eventId,
                           const QString &sourceCalendarId,
                           const QString &destinationCalendarId,
                           const AccountPtr &account,
                           QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private())
{
    d->eventsIds << eventId;
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const QStringList &eventsIds,
                           const QString &sourceCalendarId,
                           const QString &destinationCalendarId,
                           const AccountPtr &account,
                           QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private())
{
    d->eventsIds = eventsIds;
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::~EventMoveJob() = default;

void EventMoveJob::start()
{
    if (d->eventsIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString eventId = d->eventsIds.current();
    const auto request = CalendarService::prepareRequest(CalendarService::moveEventUrl(d->source, d->destination, eventId));

    enqueueRequest(request);
}

void EventMoveJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    accessManager->post(request, QByteArray());
}

KGAPI2::ObjectsList EventMoveJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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

    items << CalendarService::JSONToEvent(rawData).dynamicCast<Event>();
    d->eventsIds.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}
