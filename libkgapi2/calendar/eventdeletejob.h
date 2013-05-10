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

#ifndef LIBKGAPI2_EVENTDELETEJOB_H
#define LIBKGAPI2_EVENTDELETEJOB_H

#include <libkgapi2/deletejob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to delete one or more events from calendar in user's Google 
 *        Calendar account.
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT EventDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a new job that will delete given @p event from a
     *        calendar with given @p calendarId
     *
     * @param event Event to delete
     * @param calendarId ID of calendar in which the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventDeleteJob(const EventPtr &event, const QString &calendarId,
                            const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p events from a
     *        calendar with given @p calendarId
     *
     * @param events Events to delete
     * @param calendarId ID of calendar in which the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventDeleteJob(const EventsList &events, const QString &calendarId,
                            const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete an event with given
     *        @p eventId from a calendar with given @p calendarId
     *
     * @param eventId ID of event to delete
     * @param calendarId ID of calendar in which the events is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventDeleteJob(const QString &eventId, const QString &calendarId,
                            const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete all events with given
     *        @p eventsIds from a calendar with given @p calendarId
     *
     * @param eventsIds IDs of events to delete
     * @param calendarId ID of calendar in which the events is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventDeleteJob(const QStringList &eventIds, const QString &calendarId,
                            const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    virtual ~EventDeleteJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    virtual void handleReply(const QNetworkReply* reply, const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_EVENTDELETEJOB_H
