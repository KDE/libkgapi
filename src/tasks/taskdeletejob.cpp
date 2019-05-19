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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "taskdeletejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "task.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskDeleteJob::Private
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

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

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
    d->tasksIds.reserve(tasks.size());
    for (const TaskPtr &task : qAsConst(tasks)) {
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


