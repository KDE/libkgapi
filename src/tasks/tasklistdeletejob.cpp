/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "tasklistdeletejob.h"
#include "private/queuehelper_p.h"
#include "tasklist.h"
#include "tasksservice.h"

#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskListDeleteJob::Private
{
public:
    Private(TaskListDeleteJob *parent);
    void processNextTaskList();

    QueueHelper<QString> taskListsIds;

private:
    TaskListDeleteJob *const q;
};

TaskListDeleteJob::Private::Private(TaskListDeleteJob *parent)
    : q(parent)
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

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1StringView(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    q->enqueueRequest(request);
}

TaskListDeleteJob::TaskListDeleteJob(const TaskListPtr &taskList, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(this))
{
    d->taskListsIds << taskList->uid();
}

TaskListDeleteJob::TaskListDeleteJob(const TaskListsList &taskLists, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(this))
{
    d->taskListsIds.reserve(taskLists.size());
    for (const TaskListPtr &taskList : taskLists) {
        d->taskListsIds << taskList->uid();
    }
}

TaskListDeleteJob::TaskListDeleteJob(const QStringList &taskListsIds, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(this))
{
    d->taskListsIds = taskListsIds;
}

TaskListDeleteJob::TaskListDeleteJob(const QString &taskListsId, const AccountPtr &account, QObject *parent)
    : DeleteJob(account, parent)
    , d(new Private(this))
{
    d->taskListsIds << taskListsId;
}

TaskListDeleteJob::~TaskListDeleteJob() = default;

void TaskListDeleteJob::start()
{
    d->processNextTaskList();
}

void TaskListDeleteJob::handleReply(const QNetworkReply *reply, const QByteArray &rawData)
{
    d->taskListsIds.currentProcessed();

    KGAPI2::DeleteJob::handleReply(reply, rawData);
}

#include "moc_tasklistdeletejob.cpp"
