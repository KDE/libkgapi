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


#ifndef LIBKGAPI2_TASKDELETEJOB_H
#define LIBKGAPI2_TASKDELETEJOB_H

#include <libkgapi2/deletejob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to delete one or more tasks from tasklist in
 *        user's Google Tasks account
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT TaskDeleteJob : public KGAPI2::DeleteJob
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
    explicit TaskDeleteJob(const TaskPtr &task, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a new job that will delete given @p tasks from a
     *        tasklist with given @p taskListId
     *
     * @param tasks Tasks to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const TasksList &tasks, const QString &tasklistId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a new job that will delete task with given @p taskId
     *        from a tasklist with given @p taskListId
     *
     * @param taskId ID of task to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const QString &taskId, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a new job that will delete tasks with given @p tasksIds
     *        from a tasklist with given @p taskListId
     *
     * @param tasksIds IDs of tasks to delete
     * @param taskListId ID of tasklist in which the task is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskDeleteJob(const QStringList &tasksIds, const QString &taskListId,
                           const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~TaskDeleteJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    virtual void handleReply(const QNetworkReply* reply, const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKDELETEJOB_H
