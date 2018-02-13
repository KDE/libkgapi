/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tasksservice.h"
#include "object.h"
#include "task.h"
#include "tasklist.h"
#include "utils.h"

#include <QJsonDocument>

#include <QVariant>

namespace KGAPI2
{

namespace TasksService
{

namespace Private
{
    ObjectsList parseTaskListJSONFeed(const QVariantList &items);
    ObjectsList parseTasksJSONFeed(const QVariantList &items);

    ObjectPtr JSONToTaskList(const QVariantMap &jsonData);
    ObjectPtr JSONToTask(const QVariantMap &jsonData);

    static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));
    static const QString TasksBasePath(QStringLiteral("/tasks/v1/lists"));
    static const QString TasksListsBasePath(QStringLiteral("/tasks/v1/users/@me/lists"));
}

ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonFeed);
    if (document.isNull()) {
        return ObjectsList();
    }

    ObjectsList list;
    const QVariantMap feed = document.toVariant().toMap();

    if (feed.value(QStringLiteral("kind")).toString() == QLatin1String("tasks#taskLists")) {
        list = Private::parseTaskListJSONFeed(feed.value(QStringLiteral("items")).toList());

        if (feed.contains(QStringLiteral("nextPageToken"))) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            feedData.nextPageUrl.addQueryItem(QStringLiteral("pageToken"), feed.value(QStringLiteral("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QStringLiteral("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QStringLiteral("maxResults"), QStringLiteral("20"));
            }
        }

    } else if (feed.value(QStringLiteral("kind")).toString() == QLatin1String("tasks#tasks")) {
        list = Private::parseTasksJSONFeed(feed.value(QStringLiteral("items")).toList());

        if (feed.contains(QStringLiteral("nextPageToken"))) {
            QString taskListId = feedData.requestUrl.toString().remove(QStringLiteral("https://www.googleapis.com/tasks/v1/lists/"));
            taskListId = taskListId.left(taskListId.indexOf(QLatin1Char('/')));

            feedData.nextPageUrl = fetchAllTasksUrl(taskListId);
            feedData.nextPageUrl.addQueryItem(QStringLiteral("pageToken"), feed.value(QStringLiteral("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QStringLiteral("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QStringLiteral("maxResults"), QStringLiteral("20"));
            }
        }
    }

    return list;
}

QUrl fetchAllTasksUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks"));
    return url;
}

QUrl fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks/") % taskID);
    return url;
}

QUrl createTaskUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks"));
    return url;
}

QUrl updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks/") % taskID);
    return url;
}

QUrl removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks/") % taskID);
    return url;
}

QUrl moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % QLatin1String("/tasks/") % taskID % QLatin1String("/move"));
    if (!newParent.isEmpty()) {
        url.addQueryItem(QStringLiteral("parent"), newParent);
    }

    return url;
}

QUrl fetchTaskListsUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksListsBasePath);
    return url;
}

QUrl createTaskListUrl()
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksListsBasePath);
    return url;
}

QUrl updateTaskListUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksListsBasePath % QLatin1Char('/') % tasklistID);
    return url;
}

QUrl removeTaskListUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksListsBasePath % QLatin1Char('/') % tasklistID);
    return url;
}

/******************************* PRIVATE ******************************/

ObjectPtr Private::JSONToTaskList(const QVariantMap &jsonData)
{
    TaskListPtr taskList(new TaskList());

    taskList->setUid(jsonData.value(QStringLiteral("id")).toString());
    taskList->setEtag(jsonData.value(QStringLiteral("etag")).toString());
    taskList->setTitle(jsonData.value(QStringLiteral("title")).toString());

    return taskList.dynamicCast<Object>();
}

TaskListPtr JSONToTaskList(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();

    if (data.value(QStringLiteral("kind")).toString() == QLatin1String("tasks#taskList")) {
        return Private::JSONToTaskList(data).staticCast<TaskList>();
    }

    return TaskListPtr();
}

ObjectPtr Private::JSONToTask(const QVariantMap &jsonData)
{
    TaskPtr task(new Task());

    task->setUid(jsonData.value(QStringLiteral("id")).toString());
    task->setEtag(jsonData.value(QStringLiteral("etag")).toString());
    task->setSummary(jsonData.value(QStringLiteral("title")).toString());
    task->setLastModified(Utils::rfc3339DateFromString(jsonData.value(QStringLiteral("updated")).toString()));
    task->setDescription(jsonData.value(QStringLiteral("notes")).toString());

    if (jsonData.value(QStringLiteral("status")).toString() == QStringLiteral("needsAction")) {
        task->setStatus(KCalCore::Incidence::StatusNeedsAction);
    } else if (jsonData.value(QStringLiteral("status")).toString() == QStringLiteral("completed")) {
        task->setStatus(KCalCore::Incidence::StatusCompleted);
    } else {
        task->setStatus(KCalCore::Incidence::StatusNone);
    }

    task->setDtDue(Utils::rfc3339DateFromString(jsonData.value(QStringLiteral("due")).toString()));

    if (task->status() == KCalCore::Incidence::StatusCompleted) {
        task->setCompleted(Utils::rfc3339DateFromString(jsonData.value(QStringLiteral("completed")).toString()));
    }

    task->setDeleted(jsonData.value(QStringLiteral("deleted")).toBool());

    if (jsonData.contains(QStringLiteral("parent"))) {
        task->setRelatedTo(jsonData.value(QStringLiteral("parent")).toString(), KCalCore::Incidence::RelTypeParent);
    }

    return task.dynamicCast<Object>();
}

TaskPtr JSONToTask(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();

    if (data.value(QStringLiteral("kind")).toString() == QLatin1String("tasks#task")) {
        return Private::JSONToTask(data).staticCast<Task>();
    }

    return TaskPtr();
}

QByteArray taskListToJSON(const TaskListPtr &taskList)
{
    QVariantMap output;

    output.insert(QStringLiteral("kind"), QStringLiteral("tasks#taskList"));
    if (!taskList->uid().isEmpty()) {
        output.insert(QStringLiteral("id"), taskList->uid());
    }
    output.insert(QStringLiteral("title"), taskList->title());

    QJsonDocument document = QJsonDocument::fromVariant(output);
    return document.toJson(QJsonDocument::Compact);
}

QByteArray taskToJSON(const TaskPtr &task)
{
    QVariantMap output;

    output.insert(QStringLiteral("kind"), QStringLiteral("tasks#task"));

    if (!task->uid().isEmpty()) {
        output.insert(QStringLiteral("id"), task->uid());
    }

    output.insert(QStringLiteral("title"), task->summary());
    output.insert(QStringLiteral("notes"), task->description());

    if (!task->relatedTo(KCalCore::Incidence::RelTypeParent).isEmpty()) {
        output.insert(QStringLiteral("parent"), task->relatedTo(KCalCore::Incidence::RelTypeParent));
    }

    if (task->dtDue().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QStringLiteral("due"), task->dtDue().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss.zzzZ")));
    }

    if ((task->status() == KCalCore::Incidence::StatusCompleted) && task->completed().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QStringLiteral("completed"), task->completed().toUTC().toString(QStringLiteral("yyyy-MM-ddThh:mm:ss.zzzZ")));
        output.insert(QStringLiteral("status"), QStringLiteral("completed"));
    } else {
        output.insert(QStringLiteral("status"), QStringLiteral("needsAction"));
    }

    QJsonDocument document = QJsonDocument::fromVariant(output);
    return document.toJson(QJsonDocument::Compact);
}

ObjectsList Private::parseTaskListJSONFeed(const QVariantList &items)
{
    ObjectsList list;
    list.reserve(items.size());
    for (const QVariant &item : items) {
        list.append(Private::JSONToTaskList(item.toMap()));
    }

    return list;
}

ObjectsList Private::parseTasksJSONFeed(const QVariantList &items)
{
    ObjectsList list;
    list.reserve(items.size());
    for (const QVariant &item : items) {
        list.append(Private::JSONToTask(item.toMap()));
    }

    return list;
}

} // namespace TasksService

} // namespace KGAPI2
