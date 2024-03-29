/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief A job to fetch all tasks from given tasklist in user's Google Tasks
 *        account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * @brief Whether to fetch deleted tasks as well
     *
     * When an tasks is deleted from tasklist, it's stored as a placeholder
     * on Google server and can still be retrieved. Such task will have
     * KGAPI2::Task::deleted set to @p true.
     *
     * By default, the job will fetch deleted tasks.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see setFetchDeleted, fetchDeleted
     */
    Q_PROPERTY(bool fetchDeleted READ fetchDeleted WRITE setFetchDeleted)

    /**
     * @brief Whether to fetch completed tasks as well
     *
     * By default, the job will fetch completed tasks.
     *
     * This property does not have any effect when fetching a specific event and
     * can be modified only when the job is not running.
     *
     * @see setFetchCompleted, fetchCompleted
     */
    Q_PROPERTY(bool fetchCompleted READ fetchCompleted WRITE setFetchCompleted)

    /**
     * @brief Timestamp to fetch only tasks modified since then
     *
     * When set, this job will only fetch tasks that have been modified since
     * given timestamp.
     *
     * By default the timestamp is 0 and all tasks are fetched.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see setFetchOnlyUpdated, fetchOnlyUpdated
     */
    Q_PROPERTY(quint64 fetchOnlyUpdated READ fetchOnlyUpdated WRITE setFetchOnlyUpdated)

    /**
     * @brief Timestamp of the newest completed task that will be fetched
     *
     * Only tasks that have been completed before or precisely at the time
     * indicated by this property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see completedMax, setCompletedMax
     */
    Q_PROPERTY(quint64 completedMax READ completedMax WRITE setCompletedMax)

    /**
     * @brief Timestamp of the oldest completed task that will be fetched
     *
     * Only tasks that have been completed after or precisely at the time
     * indicated by this property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see completedMin, setCompletedMin
     */
    Q_PROPERTY(quint64 completedMin READ completedMin WRITE setCompletedMin)

    /**
     * @brief Timestamp of the newest due task that will be fetched
     *
     * Only tasks that are due before or precisely at the time indicated by
     * this property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see dueMax, setDueMax
     */
    Q_PROPERTY(quint64 dueMax READ dueMax WRITE setDueMax)

    /**
     * @brief Timestamp of the oldest due task that will be fetched
     *
     * Only tasks that are due after or precisely at the time indicated by
     * this property will be fetched.
     *
     * By default the timestamp is 0 and no limit is applied.
     *
     * This property does not have any effect when fetching a specific task and
     * can be modified only when the job is not running.
     *
     * @see dueMin, setDueMin
     */
    Q_PROPERTY(quint64 dueMin READ dueMin WRITE setDueMin)

public:
    /**
     * @brief Constructs a job that will fetch all tasks from a tasklist with
     *        given @p taskListId
     *
     * Result of this job might not contain all tasks, depending on configured
     * filters.
     *
     * @param taskListId ID of tasklist from which to fetch tasks
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskFetchJob(const QString &taskListId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will fetch a task with given @p taskId
     *        from a tasklist with given @p taskListId
     *
     * Note that none of the properties fetchDeleted, fetchCompleted,
     * fetchOnlyUpdated, completedMax, completedMin, dueMax or dueMin are applied
     * in this case.
     *
     * @param taskId ID of task to fetch
     * @param taskListId ID of tasklist in which the event is
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskFetchJob(const QString &taskId, const QString &taskListId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~TaskFetchJob() override;

    /**
     * @brief Sets whether to fetch should deleted tasks
     *
     * @param fetchDeleted
     */
    void setFetchDeleted(bool fetchDeleted = true);

    /**
     * @brief Returns whether to fetch will deleted tasks
     */
    bool fetchDeleted() const;

    /**
     * @brief Sets whether the job should fetch completed tasks
     *
     * @param fetchCompleted
     */
    void setFetchCompleted(bool fetchCompleted = true);

    /**
     * @brief Returns whether the job will fetch completed tasks
     */
    bool fetchCompleted() const;

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
     * @brief Sets timestamp of newest completed task that can be fetched.
     *
     * @param timestamp
     */
    void setCompletedMax(quint64 timestamp);

    /**
     * @brief Returns upper date limit for fetching completed tasks
     */
    quint64 completedMax() const;

    /**
     * @brief Sets timestamp of oldest completed task that can be fetched.
     *
     * @param timestamp
     */
    void setCompletedMin(quint64 timestamp);

    /**
     * @brief Returns bottom date limit for fetching completed tasks
     */
    quint64 completedMin() const;

    /**
     * @brief Sets timestamp of newest due task that can be fetched.
     *
     * @param timestamp
     */
    void setDueMax(quint64 timestamp);

    /**
     * @brief Returns upper date limit for fetching due tasks
     */
    quint64 dueMax() const;

    /**
     * @brief Sets timestamp of oldest due task that can be fetched.
     *
     * @param timestamp
     */
    void setDueMin(quint64 timestamp);

    /**
     * @brief Returns bottom date limit for fetching due tasks.
     */
    quint64 dueMin() const;

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
