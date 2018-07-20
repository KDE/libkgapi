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

#include "tasklistcreatejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "tasklist.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>


using namespace KGAPI2;

class Q_DECL_HIDDEN TaskListCreateJob::Private
{
  public:
    QueueHelper<TaskListPtr> taskLists;
};

TaskListCreateJob::TaskListCreateJob(const TaskListPtr& taskList, const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->taskLists << taskList;
}

TaskListCreateJob::TaskListCreateJob(const TaskListsList& taskLists,
                                     const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->taskLists = taskLists;
}

TaskListCreateJob::~TaskListCreateJob()
{
    delete d;
}

void TaskListCreateJob::start()
{
    if (d->taskLists.atEnd()) {
        emitFinished();
        return;
    }

    const TaskListPtr taskList = d->taskLists.current();

    const QUrl url = TasksService::createTaskListUrl();
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = TasksService::taskListToJSON(taskList);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList TaskListCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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


