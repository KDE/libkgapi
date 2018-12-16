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

EventModifyJob::~EventModifyJob()
{
    delete d;
}

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
        emitFinished();
        return;
    }

    const EventPtr event = d->events.current();
    const QUrl url = CalendarService::updateEventUrl(d->calendarId, event->uid(), d->updatesPolicy);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setRawHeader("GData-Version", CalendarService::APIVersion().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = CalendarService::eventToJSON(event);

    QStringList headers;
    auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

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
        emitFinished();
        return items;
    }

    items << CalendarService::JSONToEvent(rawData).dynamicCast<Event>();
    d->events.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}


