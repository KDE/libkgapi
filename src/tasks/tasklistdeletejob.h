/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_TASKLISTDELETEJOB_H
#define LIBKGAPI2_TASKLISTDELETEJOB_H

#include "deletejob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

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
     * @param taskListId ID of tasklist to delete
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
    ~TaskListDeleteJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start
     */
    void start() override;

    /**
     * @brief KGAPI2::Job::handleReply
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply* reply, const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKLISTDELETEJOB_H
