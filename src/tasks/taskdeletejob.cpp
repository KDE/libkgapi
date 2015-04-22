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


#include "taskdeletejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "task.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class TaskDeleteJob::Private
{
  public:
    Private(TaskDeleteJob *parent);
    void processNextTask();

    QueueHelper<QString> tasksIds;
    QString taskListId;

  private:
    TaskDeleteJob * const q;
};

TaskDeleteJob::Private::Private(TaskDeleteJob *parent):
    q(parent)
{
}

void TaskDeleteJob::Private::processNextTask()
{
    if (tasksIds.atEnd()) {
        q->emitFinished();
        return;
    }

    const QString taskId = tasksIds.current();
    const QUrl url = TasksService::removeTaskUrl(taskListId, taskId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    qCDebug(KGAPIRaw) << headers;

    q->enqueueRequest(request);
}

TaskDeleteJob::TaskDeleteJob(const TaskPtr& task, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->tasksIds << task->uid();
    d->taskListId = taskListId;
}

TaskDeleteJob::TaskDeleteJob(const TasksList& tasks, const QString& tasklistId,
                             const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const TaskPtr &task, tasks) {
        d->tasksIds << task->uid();
    }
    d->taskListId = tasklistId;
}

TaskDeleteJob::TaskDeleteJob(const QString &taskId, const QString &taskListId,
                             const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->tasksIds << taskId;
    d->taskListId = taskListId;
}

TaskDeleteJob::TaskDeleteJob(const QStringList &tasksIds, const QString &taskListId,
                             const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->tasksIds = tasksIds;
    d->taskListId = taskListId;
}

TaskDeleteJob::~TaskDeleteJob()
{
    delete d;
}

void TaskDeleteJob::start()
{
    d->processNextTask();
}

void TaskDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->tasksIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}


