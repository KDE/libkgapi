/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "modifyjob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

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
    ~TaskListModifyJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::ModifyJob::handleReplyWithItems implementation
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

