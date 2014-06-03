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

#include "tasklistdeletejob.h"
#include "tasksservice.h"
#include "account.h"
#include "debug.h"
#include "tasklist.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>

using namespace KGAPI2;

class TaskListDeleteJob::Private
{
  public:
    Private(TaskListDeleteJob *parent);
    void processNextTaskList();

    QueueHelper<QString> taskListsIds;

  private:
    TaskListDeleteJob * const q;
};

TaskListDeleteJob::Private::Private(TaskListDeleteJob *parent):
    q(parent)
{
}

void TaskListDeleteJob::Private::processNextTaskList()
{
   if (taskListsIds.atEnd()) {
        q->emitFinished();
        return;
    }

    const QString taskListId = taskListsIds.current();
    const QUrl url = TasksService::removeTaskListUrl(taskListId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    q->enqueueRequest(request);
}

TaskListDeleteJob::TaskListDeleteJob(const TaskListPtr& taskList,
                                     const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->taskListsIds << taskList->uid();
}

TaskListDeleteJob::TaskListDeleteJob(const TaskListsList& taskLists,
                                     const AccountPtr& account, QObject* parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    Q_FOREACH(const TaskListPtr &taskList, taskLists) {
        d->taskListsIds << taskList->uid();
    }
}

TaskListDeleteJob::TaskListDeleteJob(const QStringList &taskListsIds,
                                     const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->taskListsIds = taskListsIds;
}

TaskListDeleteJob::TaskListDeleteJob(const QString &taskListsId,
                                     const AccountPtr &account, QObject *parent):
    DeleteJob(account, parent),
    d(new Private(this))
{
    d->taskListsIds << taskListsId;
}

TaskListDeleteJob::~TaskListDeleteJob()
{
    delete d;
}

void TaskListDeleteJob::start()
{
    d->processNextTaskList();
}

void TaskListDeleteJob::handleReply(const QNetworkReply* reply, const QByteArray& rawData)
{
    d->taskListsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}


#include "tasklistdeletejob.moc"
