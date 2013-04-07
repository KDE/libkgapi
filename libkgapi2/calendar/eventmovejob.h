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

#ifndef KGAPI2_EVENTMOVEJOB_H
#define KGAPI2_EVENTMOVEJOB_H

#include <libkgapi2/modifyjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{

/**
 * @brief A job that moves events between given calendars
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT EventMoveJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will move given \p event from a calendar
     *        with \p sourceCalendarId to calendar with \p destinationCalendarId
     *
     * @param event Event to move
     * @param sourceCalendarId ID of calendar where the event is currently stored
     * @param destinationCalendarId ID of calendar where to move the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const EventPtr &event, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Constructs a job that will move given \p events from a calendar
     *        with \p sourceCalendarId to calendar with \p destinationCalendarId
     *
     * @param events Events to move
     * @param sourceCalendarId ID of calendar where the events are currently stored
     * @param destinationCalendarId ID of calendar where to move the events
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const EventsList &events, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Constructs a job that will move event with given \p eventId from a 
     *        calendar with \p sourceCalendarId to calendar
     *        with \p destinationCalendarId
     *
     * @param eventId ID of event to move
     * @param sourceCalendarId ID of calendar where the event is currently stored
     * @param destinationCalendarId ID of calendar where to move the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const QString &eventId, const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Constructs a job that will move events with given \p eventsIds
     *        from a calendar with \p sourceCalendarId to calendar
     *        with \p destinationCalendarId
     *
     * @param eventsIds IDs of events to move
     * @param sourceCalendarId ID of calendar where the events are currently stored
     * @param destinationCalendarId ID of calendar where to move the events
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventMoveJob(const QStringList &eventsIds,
                          const QString &sourceCalendarId,
                          const QString &destinationCalendarId,
                          const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~EventMoveJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    virtual void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType);

    /**
     * @brief KGAPI2::Job::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    virtual KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // KGAPI2_EVENTMOVEJOB_H
