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


#include "tasklistmodifyjob.h"
#include "tasksservice.h"
#include "account.h"
#include "debug.h"
#include "tasklist.h"
#include "utils.h"
#include "private/queuehelper_p.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class TaskListModifyJob::Private
{
  public:
    QueueHelper<TaskListPtr> taskLists;
};

TaskListModifyJob::TaskListModifyJob(const TaskListPtr& taskList,
                                     const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->taskLists << taskList;
}

TaskListModifyJob::TaskListModifyJob(const TaskListsList& taskLists,
                                     const AccountPtr& account, QObject* parent):
    ModifyJob(account, parent),
    d(new Private)
{
    d->taskLists = taskLists;
}

TaskListModifyJob::~TaskListModifyJob()
{
    delete d;
}

void TaskListModifyJob::start()
{
    if (d->taskLists.atEnd()) {
        emitFinished();
        return;
    }

    const TaskListPtr taskList = d->taskLists.current();

    const QUrl url = TasksService::updateTaskListUrl(taskList->uid());
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = TasksService::taskListToJSON(taskList);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    enqueueRequest(request, rawData, QLatin1String("application/json"));
}

ObjectsList TaskListModifyJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    ObjectsList items;
    if (ct != KGAPI2::JSON) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    items << TasksService::JSONToTaskList(rawData).dynamicCast<Object>();
    d->taskLists.currentProcessed();
    // Enqueue next item or finish
    start();

    return items;
}

#include "tasklistmodifyjob.moc"
