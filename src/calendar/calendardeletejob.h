/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#pragma once

#include "deletejob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

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
     * @param calendars Calendars to delete
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
    ~CalendarDeleteJob() override;

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

