/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "eventcreatejob.h"
#include "account.h"
#include "calendarservice.h"
#include "event.h"
#include "private/queuehelper_p.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

#include <KCalendarCore/Attendee>

#include <algorithm>

using namespace KGAPI2;

class Q_DECL_HIDDEN EventCreateJob::Private
{
public:
    QueueHelper<EventPtr> events;
    QString calendarId;
    SendUpdatesPolicy updatesPolicy = SendUpdatesPolicy::All;
};

EventCreateJob::EventCreateJob(const EventPtr &event, const QString &calendarId, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private)
{
    d->events << event;
    d->calendarId = calendarId;
}

EventCreateJob::EventCreateJob(const EventsList &events, const QString &calendarId, const AccountPtr &account, QObject *parent)
    : CreateJob(account, parent)
    , d(new Private)
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

    EventPtr event = d->events.current();
    QUrl requestUrl;

    // If the organizer is different from the account name, import a private copy of the event in the user's calendar,
    // or normally create it otherwise.  This prevents that Google Calendar creates a copy event when accepting invitations
    // to events created by others.
    if (!event->attendees().isEmpty() && !event->organizer().isEmpty() && event->organizer().email() != this->account()->accountName()) {
        requestUrl = CalendarService::importEventUrl(d->calendarId, d->updatesPolicy);

        // Google only allows importing an event if the owner of the target calendar is the organizer or one of
        // the attendees ("participantIsNeitherOrganizerNorAttendee"). The invitation may have been addressed to
        // a different email address of the user, so make sure the calendar owner is listed as an attendee.
        const auto attendees = event->attendees();
        const bool ownerIsAttendee = std::any_of(attendees.cbegin(), attendees.cend(), [this](const KCalendarCore::Attendee &a) {
            return a.email().compare(d->calendarId, Qt::CaseInsensitive) == 0;
        });
        if (!ownerIsAttendee && event->organizer().email().compare(d->calendarId, Qt::CaseInsensitive) != 0) {
            event = EventPtr(new Event(*event));
            KCalendarCore::Attendee owner(QString(), d->calendarId);
            // Without an explicit UID, KCalendarCore generates a random one which would be sent as the attendee ID
            owner.setUid(d->calendarId);
            owner.setStatus(KCalendarCore::Attendee::Accepted);
            event->addAttendee(owner, false);
        }
    } else {
        requestUrl = CalendarService::createEventUrl(d->calendarId, d->updatesPolicy);
    }

    const auto request = CalendarService::prepareRequest(requestUrl);
    const QByteArray rawData = CalendarService::eventToJSON(event, CalendarService::EventSerializeFlag::NoID);

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList EventCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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
