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

#ifndef LIBKGAPI2_TASKCREATEJOB_H
#define LIBKGAPI2_TASKCREATEJOB_H

#include <libkgapi2/createjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to create one or more new tasks in Google Tasks
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT TaskCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

    /**
     * @brief Specified ID of item that the newly created tasks will be
     *        subtasks of
     *
     * By default this property is empty and all tasks are created in the top
     * level of their parent tasklist
     *
     * This property can only be modified when job is not running.
     *
     * @see setParentItem, parentItem
     */
    Q_PROPERTY(QString parentItem READ parentItem WRITE setParentItem)

  public:

    /**
     * @brief Constructs a job that will create given @p task in a tasklist
     *        with id @taskListId
     *
     * @param task Task to store
     * @param taskListId ID of tasklist to create the task in
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskCreateJob(const TaskPtr& task, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a job that will create given @p tasks in a tasklist
     *        with id @taskListId
     *
     * @param tasks Tasks to store
     * @param taskListId ID of tasklist to create the task in
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskCreateJob(const TasksList& tasks, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~TaskCreateJob();

    /**
     * @brief Sets ID of parent task to create new tasks in
     *
     * @param parentId
     */
    void setParentItem(const QString &parentId);

    /**
     * @brief Returns ID of task the new items will be stored as subtasks of
     */
    QString parentItem() const;

  protected:
    virtual void start();
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // KGAPI2_TASKCREATEJOB_H
