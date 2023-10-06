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
 * @brief A job to fetch all events from given calendar in user's Google
 *        Calendar account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICALENDAR_EXPORT EventFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * @brief Whether to fetch deleted events as well
     *
     * When an event is deleted from Google Calendar, it's stored as a placeholder
     * on Google server and can still be retrieved. Such event will have
     * KGAPI2::Event::deleted set to @p true.
     *
     * By default, the job will fetch deleted events.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setFetchDeleted, fetchDeleted
     */
    Q_PROPERTY(bool fetchDeleted READ fetchDeleted WRITE setFetchDeleted)

    /**
     * @brief Timestamp to fetch only events modified since then
     *
     * When set, this job will only fetch events that have been modified since
     * given timestamp.
     *
     * By default the timestamp is 0 and all events are fetched.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setFetchOnlyUpdated, fetchOnlyUpdated
     */
    Q_PROPERTY(quint64 fetchOnlyUpdated READ fetchOnlyUpdated WRITE setFetchOnlyUpdated)

    /**
     * @brief Timestamp of the newest event that will be fetched
     *
     * Only events occurring before or precisely at the time indicated by this
     * property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setMaxTime, maxTime
     */
    Q_PROPERTY(quint64 timeMax READ timeMax WRITE setTimeMax)

    /**
     * @brief Timestamp of the oldest event that will be fetched
     *
     * Only events occurring after or precisely at the time indicated by this
     * property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setMinTime, minTime
     */
    Q_PROPERTY(quint64 timeMin READ timeMin WRITE setTimeMin)

    /**
     * @brief A filter to fetch only events matching fulltext filter
     *
     * By default the property is empty and no filter is applied.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setFilter, filter
     */
    Q_PROPERTY(QString filter READ filter WRITE setFilter)

    /**
     * @brief A token to fetch updates incrementally
     *
     * By default the property is empty. Properties timeMin, timeMax,
     * updatedMin will be ignored if sync token is specified
     *
     * @see setSyncToken, syncToken
     */
    Q_PROPERTY(QString syncToken READ syncToken WRITE setSyncToken)

public:
    /**
     * @brief Constructs a job that will fetch all events from a calendar with
     *        given @p calendarId
     *
     * Result of this job might not contain all events, depending on configured
     * filters.
     *
     * @param calendarId ID of calendar from which to fetch events
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventFetchJob(const QString &calendarId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will fetch an event with given @p eventId
     *        from a calendar with given @p calendarId
     *
     * Note that none of the filter, fetchOnlyUpdated, timeMax or timeMin properties
     * is applied in this case.
     *
     * @param eventId ID of event to fetch
     * @param calendarId ID of calendar in which the event is
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit EventFetchJob(const QString &eventId, const QString &calendarId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EventFetchJob() override;

    /**
     * @brief Sets fulltext filter.
     *
     * @param query
     */
    void setFilter(const QString &query);

    /**
     * @brief Returns fulltext filter string
     */
    [[nodiscard]] QString filter() const;

    /**
     * @brief Sets whether to fetch deleted events
     *
     * @param fetchDeleted
     */
    void setFetchDeleted(bool fetchDeleted = true);

    /**
     * @brief Returns whether deleted events are fetched.
     */
    [[nodiscard]] bool fetchDeleted();

    /**
     * @brief Sets the job to fetch only events modified since @p timestamp
     *
     * @param timestamp
     */
    void setFetchOnlyUpdated(quint64 timestamp);

    /**
     * @brief Returns whether the job will fetch only modified events
     *
     * @return 0 when all events will be fetched, a timestamp of since when the
     *         modified events will be fetched.
     */
    [[nodiscard]] quint64 fetchOnlyUpdated() const;

    /**
     * @brief Sets timestamp of newest event that can be fetched.
     *
     * @param timestamp
     */
    void setTimeMax(quint64 timestamp);

    /**
     * @brief Returns upper limit for event occurrence
     */
    [[nodiscard]] quint64 timeMax() const;

    /**
     * @brief Sets timestamp of older events that can be fetched.
     *
     * @param timestamp
     */
    void setTimeMin(quint64 timestamp);

    /**
     * @brief Returns lower boundary for events occurrence
     */
    [[nodiscard]] quint64 timeMin() const;

    /**
     * @brief Sets token for incremental updates
     *
     * @param syncToken
     */
    void setSyncToken(const QString &syncToken);

    /**
     * @brief Token for next incremental update
     */
    [[nodiscard]] QString syncToken() const;

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
