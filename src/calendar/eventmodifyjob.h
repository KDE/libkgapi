/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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

