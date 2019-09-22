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

#ifndef LIBKGAPI2_EVENTMODIFYJOB_H
#define LIBKGAPI2_EVENTMODIFYJOB_H

#include "modifyjob.h"
#include "enums.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2 {

/**
 * @brief A job to modify events in a calendar on user's Google Calendar account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT EventModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

    Q_PROPERTY(KGAPI2::SendUpdatesPolicy sendUpdates
               READ sendUpdates
               WRITE setSendUpdates
               NOTIFY sendUpdatesChanged)
  public:

    /**
     * @brief Constructs a job that will write changes in given @p event to
     *        corresponding event in calendar with given @p calendarId
     *
     * @param event Event to modify
     * @param calendarId ID of calendar where the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventModifyJob(const EventPtr &event, const QString &calendarId,
                            const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will write changes in given @p events to
     *        corresponding events in calendar with given @p calendarId
     *
     * @param events Events to modify
     * @param calendarId ID of calendar where the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventModifyJob(const EventsList &events, const QString &calendarId,
                            const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EventModifyJob() override;

    KGAPI2::SendUpdatesPolicy sendUpdates() const;
    void setSendUpdates(KGAPI2::SendUpdatesPolicy updatesPolicy);

  Q_SIGNALS:
    void sendUpdatesChanged(KGAPI2::SendUpdatesPolicy policy);

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::ModifyJob::handleReplyWithItems implementation
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

} // namespace KGAPI

#endif // LIBKGAPI2_EVENTMODIFYJOB_H
