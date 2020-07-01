/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_EVENTDELETEJOB_H
#define LIBKGAPI2_EVENTDELETEJOB_H

#include "deletejob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

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
class KGAPICALENDAR_EXPORT EventDeleteJob : public KGAPI2::DeleteJob
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
     * @param eventIds IDs of events to delete
     * @param calendarId ID of calendar in which the events is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventDeleteJob(const QStringList &eventIds, const QString &calendarId,
                            const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    ~EventDeleteJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply* reply, const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_EVENTDELETEJOB_H
