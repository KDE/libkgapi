/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "taskfetchjob.h"
#include "tasksservice.h"
#include "account.h"
#include "../debug.h"
#include "task.h"
#include "utils.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>


using namespace KGAPI2;

namespace {
    static const auto ShowDeletedParam = QStringLiteral("showDeleted");
    static const auto ShowCompletedParam = QStringLiteral("showCompleted");
    static const auto UpdatedMinParam = QStringLiteral("updatedMin");
    static const auto CompletedMinParam = QStringLiteral("completedMin");
    static const auto CompletedMaxParam = QStringLiteral("completedMax");
    static const auto DueMinParam = QStringLiteral("dueMin");
    static const auto DueMaxParam = QStringLiteral("dueMax");

    static constexpr bool FetchDeletedDefault = false;
    static constexpr bool FetchCompletedDefault = false;
}

class Q_DECL_HIDDEN TaskFetchJob::Private
{
  public:
    QString taskId;
    QString taskListId;
    bool fetchDeleted = true;
    bool fetchCompleted = true;
    quint64 updatedTimestamp;
    quint64 completedMin;
    quint64 completedMax;
    quint64 dueMin;
    quint64 dueMax;
};

TaskFetchJob::TaskFetchJob(const QString& taskListId, const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private())
{
    d->taskListId = taskListId;
}

TaskFetchJob::TaskFetchJob(const QString& taskId, const QString& taskListId,
                           const AccountPtr& account, QObject* parent):
    FetchJob(account, parent),
    d(new Private())
{
    d->taskId = taskId;
    d->taskListId = taskListId;
}

TaskFetchJob::~TaskFetchJob() = default;

void TaskFetchJob::setFetchOnlyUpdated(quint64 timestamp)
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Can't modify fetchOnlyUpdated property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify fetchCompleted property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify fetchDeleted property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify completedMin property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify completedMax property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify dueMin property when job is running";
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
        qCWarning(KGAPIDebug) << "Can't modify dueMax property when job is running";
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
        QUrlQuery query(url);
        if (d->fetchDeleted != FetchDeletedDefault) {
            query.addQueryItem(ShowDeletedParam, Utils::bool2Str(d->fetchDeleted));
        }
        if (d->fetchCompleted != FetchCompletedDefault) {
            query.addQueryItem(ShowCompletedParam, Utils::bool2Str(d->fetchCompleted));
        }
        if (d->updatedTimestamp > 0) {
            query.addQueryItem(UpdatedMinParam, Utils::ts2Str(d->updatedTimestamp));
        }
        if (d->completedMin > 0) {
            query.addQueryItem(CompletedMinParam, Utils::ts2Str(d->completedMin));
        }
        if (d->completedMax > 0) {
            query.addQueryItem(CompletedMaxParam, Utils::ts2Str(d->completedMax));
        }
        if (d->dueMin > 0) {
            query.addQueryItem(DueMinParam, Utils::ts2Str(d->dueMin));
        }
        if (d->dueMax > 0) {
            query.addQueryItem(DueMaxParam, Utils::ts2Str(d->dueMax));
        }
        url.setQuery(query);
    } else {
        url = TasksService::fetchTaskUrl(d->taskListId, d->taskId);
    }

    const QNetworkRequest request(url);
    enqueueRequest(request);
}

ObjectsList TaskFetchJob::handleReplyWithItems(const QNetworkReply *reply, const QByteArray& rawData)
{
    FeedData feedData;
    feedData.requestUrl = reply->url();

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
        setErrorString(tr("Invalid response content type"));
        emitFinished();
        return items;
    }

    if (feedData.nextPageUrl.isValid()) {
        const QNetworkRequest request(feedData.nextPageUrl);
        enqueueRequest(request);
    }

    return items;
}


