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

#ifndef LIBKGAPI2_TASKLISTMODIFYJOB_H
#define LIBKGAPI2_TASKLISTMODIFYJOB_H

#include "modifyjob.h"
#include "kgapitasks_export.h"

namespace KGAPI2 {

/**
 * @brief A job to modify tasklists in user's Google Tasks account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskListModifyJob : public KGAPI2::ModifyJob
{

    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will write changes in given @p taskList to
     *        corresponding tasklist in user's Google Tasks account
     *
     * @param taskList Tasklist to modify
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListModifyJob(const TaskListPtr &taskList,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will write changes in given @p taskLists to
     *        corresponding tasklists in user's Google Tasks account
     *
     * @param taskLists Tasklists to modify
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListModifyJob(const TaskListsList &taskLists,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    virtual ~TaskListModifyJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() Q_DECL_OVERRIDE;

    /**
     * @brief KGAPI2::ModifyJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKLISTMODIFYJOB_H
