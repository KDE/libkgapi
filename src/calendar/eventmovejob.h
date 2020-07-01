/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_EVENTMOVEJOB_H
#define KGAPI2_EVENTMOVEJOB_H

#include "modifyjob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief A job that moves events between given calendars
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT EventMoveJob : public KGAPI2::ModifyJob
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
                          const AccountPtr &account, QObject *parent = nullptr);

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
                          const AccountPtr &account, QObject *parent = nullptr);

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
                          const AccountPtr &account, QObject *parent = nullptr);

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
                          const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EventMoveJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;

    /**
     * @brief KGAPI2::Job::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // KGAPI2_EVENTMOVEJOB_H
