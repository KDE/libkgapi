/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "taskdeletejob.h"
#include "account.h"
#include "debug.h"
#include "private/queuehelper_p.h"
#include "task.h"
#include "tasksservice.h"
#include "utils.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskDeleteJob::Private
{
public:
    QueueHelper<QString> tasksIds;
    QString taskListId;
};

TaskDeleteJob::TaskDeleteJob(const TaskPtr &task, const QString &taskListId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private())
{
    d->tasksIds << task->uid();
    d->taskListId = taskListId;
}

TaskDeleteJob::TaskDeleteJob(const TasksList &tasks, const QString &tasklistId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private())
{
    d->tasksIds.reserve(tasks.size());
    for (const TaskPtr &task : std::as_const(tasks)) {
        d->tasksIds << task->uid();
    }
    d->taskListId = tasklistId;
}

TaskDeleteJob::TaskDeleteJob(const QString &taskId, const QString &taskListId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private())
{
    d->tasksIds << taskId;
    d->taskListId = taskListId;
}

TaskDeleteJob::TaskDeleteJob(const QStringList &tasksIds, const QString &taskListId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private())
{
    d->tasksIds = tasksIds;
    d->taskListId = taskListId;
}

TaskDeleteJob::~TaskDeleteJob() = default;

void TaskDeleteJob::start()
{
    if (d->tasksIds.atEnd()) {
        emitFinished();
        return;
    }

    const QString taskId = d->tasksIds.current();
    const QUrl url = TasksService::removeTaskUrl(d->taskListId, taskId);
    QNetworkRequest request(url);

    enqueueRequest(request);
}

void TaskDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    d->tasksIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}

#include "moc_taskdeletejob.cpp"
