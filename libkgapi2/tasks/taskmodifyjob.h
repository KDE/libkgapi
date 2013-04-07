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


#ifndef LIBKGAPI2_TASKMODIFYJOB_H
#define LIBKGAPI2_TASKMODIFYJOB_H

#include <libkgapi2/modifyjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to modify tasks in a tasklist in user's Google Tasks account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT TaskModifyJob : public KGAPI2::ModifyJob
{

    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will write changes in given @p task to
     *        corresponding task in a tasklist with given @p taskListId
     *
     * @param task Task to modify
     * @param taskListId ID of tasklist where the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskModifyJob(const TaskPtr &task, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a job that will write changes in given @p tasks to
     *        corresponding tasks in tasklist with given @p taskListId
     *
     * @param tasks Tasks to modify
     * @param taskListId ID of tasklist where the event is stored
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskModifyJob(const TasksList &tasks, const QString &taskListId,
                           const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~TaskModifyJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::ModifyJob::handleReplyWithItems implementation
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

#endif // LIBKGAPI2_TASKMODIFYJOB_H
