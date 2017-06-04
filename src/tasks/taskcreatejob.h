/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBKGAPI2_TASKCREATEJOB_H
#define LIBKGAPI2_TASKCREATEJOB_H

#include "createjob.h"
#include "kgapitasks_export.h"

namespace KGAPI2 {

/**
 * @brief A job to create one or more new tasks in Google Tasks
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskCreateJob : public KGAPI2::CreateJob
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
                           const AccountPtr &account, QObject* parent = nullptr);

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
                           const AccountPtr &account, QObject* parent = nullptr);

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
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // KGAPI2_TASKCREATEJOB_H
