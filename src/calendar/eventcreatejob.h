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

#ifndef LIBKGAPI2_EVENTCREATEJOB_H
#define LIBKGAPI2_EVENTCREATEJOB_H

#include "createjob.h"
#include "enums.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2 {

/**
 * @brief A job to create one or more new events in given calendar 
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT EventCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

    Q_PROPERTY(KGAPI2::SendUpdatesPolicy sendUpdates
               READ sendUpdates
               WRITE setSendUpdates
               NOTIFY sendUpdatesChanged)
  public:
    /**
     * @brief Constructs a job that will create given @p event in a calendar
     *        with given @p calendarID
     *
     * @param event Event to create
     * @param calendarId ID of calendar in which to store the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventCreateJob(const EventPtr &event, const QString &calendarId,
                            const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will create given @p events in a calendar
     *        with given @p calendarID
     *
     * @param events Events to create
     * @param calendarId ID of calendar in which to store the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventCreateJob(const EventsList &events, const QString &calendarId,
                            const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EventCreateJob() override;

    KGAPI2::SendUpdatesPolicy sendUpdates() const;
    void setSendUpdates(KGAPI2::SendUpdatesPolicy updatePolicy);

  Q_SIGNALS:
    void sendUpdatesChanged(KGAPI2::SendUpdatesPolicy policy);

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::CreateJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_EVENTCREATEJOB_H
