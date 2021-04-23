/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "createjob.h"
#include "kgapitasks_export.h"

#include <QScopedPointer>

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

    /**
     * @brief Previous sibling task identifier. If the task is created at the
     * first position among its siblings, this parameter is omitted.
     *
     * This property can only be modified when job is not running.
     *
     * @see setPrevious, previous
     */
    Q_PROPERTY(QString previous READ previous WRITE setPrevious)

  public:

    /**
     * @brief Constructs a job that will create given @p task in a tasklist
     *        with id @p taskListId
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
     *        with id @p taskListId
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
    ~TaskCreateJob() override;

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

    /**
     * @brief Sets previous sibling task identifier. If the task is created at the
     * first position among its siblings, this parameter is omitted.
     *
     * @param previousId
     */
    void setPrevious(const QString &previousId);

    /**
     * @brief Previous sibling task identifier. If the task is created at the
     * first position among its siblings, this parameter is omitted.
     */
    QString previous() const;

  protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData) override;

  private:
    class Private;
    QScopedPointer<Private> const d;
    friend class Private;
};

} // namespace KGAPI2

