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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBKGAPI2_CALENDARDELETEJOB_H
#define LIBKGAPI2_CALENDARDELETEJOB_H

#include "deletejob.h"
#include "kgapicalendar_export.h"

namespace KGAPI2 {

/**
 * @brief A job to delete one or more calendars from user's Google Calendar
 *        account.
 *
 * Note that all events in these calendars will be removed too. All operations
 * are irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT CalendarDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a new job that will delete given @p calendar from user's
     *        Google Calendar account
     *
     * @param calendar Calendar to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarDeleteJob(const CalendarPtr &calendar,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p calendars from user's
     *        Google Calendar account
     *
     * @param calendar Calendar to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarDeleteJob(const CalendarsList &calendars,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete calendar with given 
     *        @p calendarId from user's Google Calendar account
     *
     * @param calendarId ID of calendar to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarDeleteJob(const QString &calendarId,
                               const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete calendars with given
     *        @p calendarsIds from user's Google Calendar account
     *
     * @param calendarsIds IDs of calendar to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarDeleteJob(const QStringList &calendarsIds,
                               const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    virtual ~CalendarDeleteJob();

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
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CALENDARDELETEJOB_H
