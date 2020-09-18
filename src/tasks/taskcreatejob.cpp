/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

namespace {
    static const auto ParentParam = QStringLiteral("parent");
    static const auto PreviousParam = QStringLiteral("previous");
}

class Q_DECL_HIDDEN TaskCreateJob::Private
{
  public:
    QueueHelper<TaskPtr> tasks;
    QString taskListId;
    QString parentId;
    QString previousId;
};

TaskCreateJob::TaskCreateJob(const TaskPtr& task, const QString& taskListId,
                             const AccountPtr& account, QObject* parent):
    CreateJob(account, parent),
    d(new Private)
{
    d->tasks.enqueue(task);
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

TaskCreateJob::~TaskCreateJob() = default;

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

QString TaskCreateJob::previous() const
{
    return d->previousId;
}

void TaskCreateJob::setPrevious(const QString &previousId)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify previous property when job is running!";
        return;
    }

    d->previousId = previousId;
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
        query.addQueryItem(ParentParam, d->parentId);
    }
    if (!d->previousId.isEmpty()) {
        query.addQueryItem(PreviousParam, d->previousId);
    }
    url.setQuery(query);
    QNetworkRequest request(url);

    const QByteArray rawData = TasksService::taskToJSON(task);

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


