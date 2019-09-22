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

#ifndef LIBKGAPI2_CALENDARCREATEJOB_H
#define LIBKGAPI2_CALENDARCREATEJOB_H

#include "createjob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2 {

/**
 * @brief A job to create one or more new calendars in user's Google Calendar
 *       account
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT CalendarCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will create given @p calendar in user's
     *        Google Calendar account
     *
     * @param calendar Calendar to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarCreateJob(const CalendarPtr &calendar,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will create given @p calendars in user's
     *        Google Calendar account
     *
     * @param calendars Calendars to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarCreateJob(const CalendarsList &calendars,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CalendarCreateJob() override;

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
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CALENDARCREATEJOB_H
