/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_TASKLISTCREATEJOB_H
#define LIBKGAPI2_TASKLISTCREATEJOB_H

#include "createjob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

namespace KGAPI2 {

/**
 * @brief A job to create one or more new tasklists in user's Google Tasks account
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPITASKS_EXPORT TaskListCreateJob : public KGAPI2::CreateJob
{

    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will create given @p taskList in user's 
     *        Google Tasks account
     *
     * @param taskList Tasklist to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListCreateJob(const TaskListPtr &taskList,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will create given @p taskLists in user's 
     *        Google Tasks account
     *
     * @param taskLists Tasklists to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit TaskListCreateJob(const TaskListsList &taskLists,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~TaskListCreateJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::CreateJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_TASKLISTCREATEJOB_H
