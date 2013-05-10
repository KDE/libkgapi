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

#include "eventmovejob.h"
#include "account.h"
#include "calendarservice.h"
#include "debug.h"
#include "event.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class EventMoveJob::Private
{
  public:
    Private(EventMoveJob *parent);
    void processNextEvent();

    QueueHelper<QString> eventsIds;
    QString source;
    QString destination;

  private:
    EventMoveJob * const q;
};

EventMoveJob::Private::Private(EventMoveJob *parent):
    q(parent)
{
}

void EventMoveJob::Private::processNextEvent()
{
   if (eventsIds.atEnd()) {
        q->emitFinished();
        return;
    }

    const QString eventId = eventsIds.current();
    const QUrl url = CalendarService::moveEventUrl(source, destination, eventId);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    request.setUrl(url);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    q->enqueueRequest(request);
}

EventMoveJob::EventMoveJob(const EventPtr &event, const QString &sourceCalendarId,
                           const QString &destinationCalendarId, const AccountPtr &account,
                           QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->eventsIds << event->uid();
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const EventsList &events, const QString &sourceCalendarId,
                           const QString &destinationCalendarId, const AccountPtr &account,
                           QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const EventPtr &event, events) {
        d->eventsIds << event->uid();
    }
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const QString &eventId, const QString &sourceCalendarId,
                           const QString &destinationCalendarId, const AccountPtr &account,
                           QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->eventsIds << eventId;
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::EventMoveJob(const QStringList &eventsIds, const QString &sourceCalendarId,
                           const QString &destinationCalendarId, const AccountPtr &account,
                           QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->eventsIds = eventsIds;
    d->source = sourceCalendarId;
    d->destination = destinationCalendarId;
}

EventMoveJob::~EventMoveJob()
{
    delete d;
}

void EventMoveJob::start()
{
    d->processNextEvent();
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
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << CalendarService::JSONToEvent(rawData).dynamicCast<Event>();
    d->eventsIds.currentProcessed();
    // Enqueue next item or finish
    d->processNextEvent();

    return items;
}


#include "eventmovejob.moc"
