/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "tasklistmodifyjob.h"
#include "private/queuehelper_p.h"
#include "tasklist.h"
#include "tasksservice.h"
#include "utils.h"

#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskListModifyJob::Private
{
public:
    QueueHelper<TaskListPtr> taskLists;
};

TaskListModifyJob::TaskListModifyJob(const TaskListPtr &taskList, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private)
{
    d->taskLists << taskList;
}

TaskListModifyJob::TaskListModifyJob(const TaskListsList &taskLists, const AccountPtr &account, QObject *parent)
    : ModifyJob(account, parent)
    , d(new Private)
{
    d->taskLists = taskLists;
}

TaskListModifyJob::~TaskListModifyJob() = default;

void TaskListModifyJob::start()
{
    if (d->taskLists.atEnd()) {
        emitFinished();
        return;
    }

    const TaskListPtr taskList = d->taskLists.current();

    const QUrl url = TasksService::updateTaskListUrl(taskList->uid());
    QNetworkRequest request(url);

    const QByteArray rawData = TasksService::taskListToJSON(taskList);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : std::as_const(rawHeaderList)) {
        headers << QLatin1StringView(str) + QLatin1StringView(": ") + QLatin1StringView(request.rawHeader(str));
    }

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList TaskListModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData)
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

    items << TasksService::JSONToTaskList(rawData).dynamicCast<Object>();
    d->taskLists.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}

#include "moc_tasklistmodifyjob.cpp"
