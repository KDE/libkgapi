/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "modifyjob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

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
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2

