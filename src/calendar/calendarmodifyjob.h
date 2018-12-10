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

#ifndef LIBKGAPI2_CALENDARMODIFYJOB_H
#define LIBKGAPI2_CALENDARMODIFYJOB_H

#include "modifyjob.h"
#include "kgapicalendar_export.h"

namespace KGAPI2 {

/**
 * @brief A job to modify calendars on user's Google Calendar account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT CalendarModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will write changes in given @p calendar to
     *        corresponding calendar on user's Google Calendar account
     *
     * @param calendar Calendar to modify
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarModifyJob(const CalendarPtr &calendar,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will write changes in given @p calendars to
     *        corresponding calendars on user's Google Calendar account
     *
     * @param calendars Calendars to modify
     * @param account Account to authenticate requests
     * @param parent
     */
    explicit CalendarModifyJob(const CalendarsList &calendars,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CalendarModifyJob() override;

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
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_CALENDARMODIFYJOB_H
