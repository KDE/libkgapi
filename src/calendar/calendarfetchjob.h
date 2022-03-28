/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief A job to fetch calendars from user's Google Calendar account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT CalendarFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a job that will fetch all calendars from user's
     *        Google Calendar account
     *
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarFetchJob(const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will fetch a single calendar with given
     *        @p calendarId from user's Google Calendar account
     *
     * @param calendarId ID of calendar to fetch
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit CalendarFetchJob(const QString &calendarId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CalendarFetchJob() override;

protected:
    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::FetchJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2
