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

#include "taskfetchjob.h"
#include "tasksservice.h"
#include "account.h"
#include "debug.h"
#include "task.h"
#include "utils.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <KDE/KLocalizedString>

using namespace KGAPI2;

class TaskFetchJob::Private
{
  public:
    Private(TaskFetchJob *parent);
    QNetworkRequest createRequest(const QUrl &url);

    QString taskId;
    QString taskListId;
    bool fetchDeleted;
    bool fetchCompleted;
    quint64 updatedTimestamp;
    quint64 completedMin;
    quint64 completedMax;
    quint64 dueMin;
    quint64 dueMax;

  private:
    TaskFetchJob * const q;
};

TaskFetchJob::Private::Private(TaskFetchJob* parent):
    fetchDeleted(true),
    fetchCompleted(true),
    updatedTimestamp(0),
    completedMin(0),
    completedMax(0),
    dueMin(0),
    dueMax(0),
    q(parent)
{
}

QNetworkRequest TaskFetchJob::Private::createRequest(const QUrl& url)
{
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer " + q->account()->accessToken().toLatin1());
    request.setUrl(url);

    QStringList headers;
    Q_FOREACH(const QByteArray &str, request.rawHeaderList()) {
        headers << QLatin1String(str) + QLatin1String(": ") + QLatin1String(request.rawHeader(str));
    }
    KGAPIDebugRawData() << headers;

    return request;
}

TaskFetchJob::TaskFetchJob(const QString& taskListId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->taskListId = taskListId;
}

TaskFetchJob::TaskFetchJob(const QString& taskId, const QString& taskListId,
                           const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private(this))
{
    d->taskId = taskId;
    d->taskListId = taskListId;
}

TaskFetchJob::~TaskFetchJob()
{
    delete d;
}

void TaskFetchJob::setFetchOnlyUpdated(quint64 timestamp)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify fetchOnlyUpdated property when job is running";
        return;
    }
    d->updatedTimestamp = timestamp;
}

quint64 TaskFetchJob::fetchOnlyUpdated()
{
    return d->updatedTimestamp;
}

void TaskFetchJob::setFetchCompleted(bool fetchCompleted)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify fetchCompleted property when job is running";
        return;
    }
    d->fetchCompleted = fetchCompleted;
}

bool TaskFetchJob::fetchCompleted() const
{
    return d->fetchCompleted;
}

void TaskFetchJob::setFetchDeleted(bool fetchDeleted)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify fetchDeleted property when job is running";
        return;
    }
    d->fetchDeleted = fetchDeleted;
}

bool TaskFetchJob::fetchDeleted() const
{
    return d->fetchDeleted;
}

void TaskFetchJob::setCompletedMin(quint64 timestamp)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify completedMin property when job is running";
        return;
    }
    d->completedMin = timestamp;
}

quint64 TaskFetchJob::completedMin() const
{
    return d->completedMin;
}

void TaskFetchJob::setCompletedMax(quint64 timestamp)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify completedMax property when job is running";
        return;
    }
    d->completedMax = timestamp;
}

quint64 TaskFetchJob::completedMax() const
{
    return d->completedMax;
}

void TaskFetchJob::setDueMin(quint64 timestamp)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify dueMin property when job is running";
        return;
    }
    d->dueMin = timestamp;
}

quint64 TaskFetchJob::dueMin() const
{
    return d->dueMin;
}

void TaskFetchJob::setDueMax(quint64 timestamp)
{
    if (isRunning()) {
        KGAPIWarning() << "Can't modify dueMax property when job is running";
        return;
    }
    d->dueMax = timestamp;
}

quint64 TaskFetchJob::dueMax() const
{
    return d->dueMax;
}

void TaskFetchJob::start()
{
    QUrl url;
    if (d->taskId.isEmpty()) {
        url = TasksService::fetchAllTasksUrl(d->taskListId);
        url.addQueryItem(QLatin1String("showDeleted"), Utils::bool2Str(d->fetchDeleted));
        url.addQueryItem(QLatin1String("showCompleted"), Utils::bool2Str(d->fetchCompleted));
        if (d->updatedTimestamp > 0) {
            url.addQueryItem(QLatin1String("updatedMin"), Utils::ts2Str(d->updatedTimestamp));
        }
        if (d->completedMin > 0) {
            url.addQueryItem(QLatin1String("completedMin"), Utils::ts2Str(d->completedMin));
        }
        if (d->completedMax > 0) {
            url.addQueryItem(QLatin1String("completedMax"), Utils::ts2Str(d->completedMax));
        }
        if (d->dueMin > 0) {
            url.addQueryItem(QLatin1String("dueMin"), Utils::ts2Str(d->dueMin));
        }
        if (d->dueMax > 0) {
            url.addQueryItem(QLatin1String("dueMax"), Utils::ts2Str(d->dueMax));
        }
    } else {
        url = TasksService::fetchTaskUrl(d->taskListId, d->taskId);
    }

    const QNetworkRequest request = d->createRequest(url);
    enqueueRequest(request);
}

ObjectsList TaskFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->request().url();

    ObjectsList items;
    QString itemId;
    const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    ContentType ct = Utils::stringToContentType(contentType);
    if (ct == KGAPI2::JSON) {
        if (d->taskId.isEmpty()) {
            items =  TasksService::parseJSONFeed(rawData, feedData);
        } else {
            items << TasksService::JSONToTask(rawData);
        }
    } else {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request = d->createRequest(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}

#include "taskfetchjob.moc"
