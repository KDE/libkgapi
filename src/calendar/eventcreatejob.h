/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "createjob.h"
#include "enums.h"
#include "kgapicalendar_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief A job to create one or more new events in given calendar
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT EventCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

    Q_PROPERTY(KGAPI2::SendUpdatesPolicy sendUpdates READ sendUpdates WRITE setSendUpdates NOTIFY sendUpdatesChanged)
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
    explicit EventCreateJob(const EventPtr &event, const QString &calendarId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will create given @p events in a calendar
     *        with given @p calendarID
     *
     * @param events Events to create
     * @param calendarId ID of calendar in which to store the event
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventCreateJob(const EventsList &events, const QString &calendarId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EventCreateJob() override;

    Q_REQUIRED_RESULT KGAPI2::SendUpdatesPolicy sendUpdates() const;
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
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2
