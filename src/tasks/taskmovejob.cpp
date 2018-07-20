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

#include "taskmovejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "task.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskMoveJob::Private
{
  public:
    Private(TaskMoveJob *parent);
    void processNextTask();

    QueueHelper<QString> tasksIds;
    QString taskListId;
    QString newParentId;

  private:
    TaskMoveJob * const q;
};

TaskMoveJob::Private::Private(TaskMoveJob *parent):
    q(parent)
{
}

void TaskMoveJob::Private::processNextTask()
{
    if (tasksIds.atEnd()) {
        q->emitFinished();
        return;
    }

    const QString taskId = tasksIds.current();
    const QUrl url = TasksService::moveTaskUrl(taskListId, taskId, newParentId);
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

TaskMoveJob::TaskMoveJob(const TaskPtr& task, const QString& taskListId,
                         const QString& newParentId, const AccountPtr& account,
                         QObject* parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->tasksIds << task->uid();
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const TasksList& tasks, const QString& taskListId,
                         const QString& newParentId, const AccountPtr& account,
                         QObject* parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->tasksIds.reserve(tasks.size());
    for (const TaskPtr &task : tasks) {
        d->tasksIds << task->uid();
    }
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const QString &taskId, const QString &taskListId,
                         const QString &newParentId, const AccountPtr &account,
                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->tasksIds << taskId;
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const QStringList &tasksIds, const QString &taskListId,
                         const QString &newParentId, const AccountPtr &account,
                         QObject *parent):
    ModifyJob(account, parent),
    d(new Private(this))
{
    d->tasksIds = tasksIds;
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::~TaskMoveJob()
{
    delete d;
}

void TaskMoveJob::start()
{
    d->processNextTask();
}

void TaskMoveJob::dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType)
{
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    accessManager->post(request, QByteArray());
}

void TaskMoveJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    d->tasksIds.currentProcessed();
    d->processNextTask();
}


