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

#include "taskcreatejob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "utils.h"
#include "task.h"
#include "private/queuehelper_p.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace KGAPI2;

class Q_DECL_HIDDEN TaskCreateJob::Private
{
  public:
    QueueHelper<TaskPtr> tasks;
    QString taskListId;
    QString parentId;
};

TaskCreateJob::TaskCreateJob(const TaskPtr& task, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->tasks << task;
    d->taskListId = taskListId;
}

TaskCreateJob::TaskCreateJob(const TasksList& tasks, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->tasks = tasks;
    d->taskListId = taskListId;
}

TaskCreateJob::~TaskCreateJob()
{
    delete d;
}

QString TaskCreateJob::parentItem() const
{
    return d->parentId;
}

void TaskCreateJob::setParentItem(const QString &parentId)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify parentItem property when job is running!";
        return;
    }

    d->parentId = parentId;
}

void TaskCreateJob::start()
{
   if (d->tasks.atEnd()) {
        emitFinished();
        return;
    }

    const TaskPtr task = d->tasks.current();

    QUrl url = TasksService::createTaskUrl(d->taskListId);
    QUrlQuery query(url);
    if (!d->parentId.isEmpty()) {
        query.addQueryItem(QStringLiteral("parent"), d->parentId);
    }
    url.setQuery(query);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + account()->accessToken().toLatin1());
    request.setUrl(url);

    const QByteArray rawData = TasksService::taskToJSON(task);

    QStringList headers;
    const auto rawHeaderList = request.rawHeaderList();
    headers.reserve(rawHeaderList.size());
    for (const QByteArray &str : qAsConst(rawHeaderList)) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }

    enqueueRequest(request, rawData, QStringLiteral("application/json"));
}

ObjectsList TaskCreateJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
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


