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

#ifndef LIBKGAPI2_TASKLISTDELETEJOB_H
#define LIBKGAPI2_TASKLISTDELETEJOB_H

#include "deletejob.h"
#include "kgapitasks_export.h"

namespace KGAPI2 {

/**
 * @brief A job to delete one or more tasklists from user's Google Calendar
 *        account.
 *
 * Note that all tasks in these taskslists will be removed too. All operations
 * are irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskListDeleteJob : public KGAPI2::DeleteJob
{

    Q_OBJECT

  public:

    /**
     * @brief Constructs a new job that will delete given @p taskList from user's
     *        Google Tasks account
     *
     * @param taskList Tasklist to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListDeleteJob(const TaskListPtr &taskList,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p taskLists from user's
     *        Google Tasks account
     *
     * @param taskLists Tasklists to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListDeleteJob(const TaskListsList &taskLists,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete tasklist with given
     *       @p taskListId from user's Google Tasks account
     *
     * @param taskList ID of tasklist to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListDeleteJob(const QString &taskListId,
                               const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete tasklists with given 
     *        @p taskListsIds from user's Google Tasks account
     *
     * @param taskListsIds IDs of tasklists to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListDeleteJob(const QStringList &taskListsIds,
                               const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    virtual ~TaskListDeleteJob();

  protected:

    /**
     * @brief KGAPI2::Job::start
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::handleReply
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

#endif // LIBKGAPI2_TASKLISTDELETEJOB_H
