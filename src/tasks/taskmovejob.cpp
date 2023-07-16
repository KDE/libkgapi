/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "taskmovejob.h"
#include "private/queuehelper_p.h"
#include "task.h"
#include "tasksservice.h"

#include <QNetworkReply>
#include <QNetworkRequest>

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
    TaskMoveJob *const q;
};

TaskMoveJob::Private::Private(TaskMoveJob *parent)
    : q(parent)
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

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    q->enqueueRequest(request);
}

TaskMoveJob::TaskMoveJob(const TaskPtr &task, const QString &taskListId, const QString &newParentId, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->tasksIds << task->uid();
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const TasksList &tasks, const QString &taskListId, const QString &newParentId, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->tasksIds.reserve(tasks.size());
    for (const TaskPtr &task : tasks) {
        d->tasksIds << task->uid();
    }
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const QString &taskId, const QString &taskListId, const QString &newParentId, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->tasksIds << taskId;
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::TaskMoveJob(const QStringList &tasksIds, const QString &taskListId, const QString &newParentId, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private(this))
{
    d->tasksIds = tasksIds;
    d->taskListId = taskListId;
    d->newParentId = newParentId;
}

TaskMoveJob::~TaskMoveJob() = default;

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

void TaskMoveJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    Q_UNUSED(reply)
    Q_UNUSED(rawData)

    d->tasksIds.currentProcessed();
    d->processNextTask();
}

#include "moc_taskmovejob.cpp"
