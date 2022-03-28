/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief A job to delete one or more tasks from tasklist in
 *        user's Google Tasks account
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new job that will delete given @p task from a
     *        tasklist with given @p taskListId
     *
     * @param task Task to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const TaskPtr &task, const QString &taskListId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a new job that will delete given @p tasks from a
     *        tasklist with given @p taskListId
     *
     * @param tasks Tasks to delete
     * @param tasklistId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const TasksList &tasks, const QString &tasklistId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a new job that will delete task with given @p taskId
     *        from a tasklist with given @p taskListId
     *
     * @param taskId ID of task to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const QString &taskId, const QString &taskListId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a new job that will delete tasks with given @p tasksIds
     *        from a tasklist with given @p taskListId
     *
     * @param tasksIds IDs of tasks to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const QStringList &tasksIds, const QString &taskListId, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~TaskDeleteJob() override;

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
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2
