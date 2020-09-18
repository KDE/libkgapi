/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "taskmodifyjob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "task.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN TaskModifyJob::Private
{
  public:
    QueueHelper<TaskPtr> tasks;
    QString taskListId;
};

TaskModifyJob::TaskModifyJob(const TaskPtr& task, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->tasks.enqueue(task);
    d->taskListId = taskListId;
}

TaskModifyJob::TaskModifyJob(const TasksList& tasks, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->tasks = tasks;
    d->taskListId = taskListId;
}

TaskModifyJob::~TaskModifyJob() = default;

void TaskModifyJob::start()
{
    if (d->tasks.atEnd()) {
        emitFinished();
        return;
    }

    const TaskPtr task = d->tasks.current();
    const QUrl url = TasksService::updateTaskUrl(d->taskListId, task->uid());
    QNetworkRequest request(url);

    const QByteArray rawData = TasksService::taskToJSON(task);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList TaskModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << TasksService::JSONToTask(rawData).dynamicCast<Object>();
    d->tasks.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}


