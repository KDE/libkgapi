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

#include "tasklistdeletejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "tasklist.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskListDeleteJob::Private
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
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

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
    d->taskListsIds.reserve(taskLists.size());
    for (const TaskListPtr &taskList : taskLists) {
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



