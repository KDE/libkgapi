/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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

