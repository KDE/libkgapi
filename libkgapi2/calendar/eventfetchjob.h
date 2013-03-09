/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_EVENTFETCHJOB_H
#define LIBKGAPI2_EVENTFETCHJOB_H

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to fetch all events from given calendar in user's Google 
 *        Calendar account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT EventFetchJob : public KGAPI2::FetchJob
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
     * Only events ocurring after or precisely at the time indicated by this
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
    explicit EventFetchJob(const QString &calendarId, const AccountPtr &account,
                           QObject* parent = 0);


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
    explicit EventFetchJob(const QString &eventId, const QString &calendarId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~EventFetchJob();

    /**
     * @brief Sets fulltext filter.
     *
     * @param query
     */
    void setFilter(const QString &query);

    /**
     * @brief Returns fulltext filter string
     */
    QString filter() const;

    /**
     * @brief Sets whether to fetch deleted events
     *
     * @param fetchDeleted
     */
    void setFetchDeleted(bool fetchDeleted = true);

    /**
     * @brief Returns whether deleted events are fetched.
     */
    bool fetchDeleted();

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
    quint64 fetchOnlyUpdated();

    /**
     * @brief Sets timestamp of newest event that can be fetched.
     *
     * @param timestamp
     */
    void setTimeMax(quint64 timestamp);

    /**
     * @brief Returns upper limit for event occurrence
     */
    quint64 timeMax() const;

    /**
     * @brief Sets timestamp of older events that can be fetched.
     *
     * @param timestamp
     */
    void setTimeMin(quint64 timestamp);

    /**
     * @brief Returns lower boundary for events occurrence
     */
    quint64 timeMin() const;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::FetchJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_EVENTFETCHJOB_H
