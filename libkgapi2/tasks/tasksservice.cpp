/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tasksservice.h"
#include "object.h"
#include "task.h"
#include "tasklist.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <KUrl>
#include <QtCore/QVariant>

namespace KGAPI2
{

namespace TasksService
{

namespace Private
{
    ObjectsList parseTaskListJSONFeed(const QVariantList &items);
    ObjectsList parseTasksJSONFeed(const QVariantList &items);

    ObjectPtr JSONToTaskList(QVariantMap jsonData);
    ObjectPtr JSONToTask(QVariantMap jsonData);
}

ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    ObjectsList list;
    const QVariantMap feed = parser.parse(jsonFeed).toMap();

    if (feed.value(QLatin1String("kind")).toString() == QLatin1String("tasks#taskLists")) {
        list = Private::parseTaskListJSONFeed(feed.value(QLatin1String("items")).toList());

        if (feed.contains(QLatin1String("nextPageToken"))) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), feed.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
            }
        }

    } else if (feed.value(QLatin1String("kind")).toString() == QLatin1String("tasks#tasks")) {
        list = Private::parseTasksJSONFeed(feed.value(QLatin1String("items")).toList());

        if (feed.contains(QLatin1String("nextPageToken"))) {
            QString taskListId = feedData.requestUrl.toString().remove(QLatin1String("https://www.googleapis.com/tasks/v1/lists/"));
            taskListId = taskListId.left(taskListId.indexOf(QLatin1Char('/')));

            feedData.nextPageUrl = fetchAllTasksUrl(taskListId);
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), feed.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
            }
        }
    }

    return list;
}

QUrl fetchAllTasksUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));

    return url;
}

QUrl fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl createTaskUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));

    return url;
}

QUrl updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);
    url.addPath(QLatin1String("move"));

    if (!newParent.isEmpty()) {
        url.addQueryItem(QLatin1String("parent"), newParent);
    }

    return url;
}

QUrl fetchTaskListsUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/tasks/v1/users/@me/lists"));
}

QUrl createTaskListUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/tasks/v1/users/@me/lists"));
}

QUrl updateTaskListUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/users/@me/lists/");
    url.addPath(tasklistID);

    return url;
}

QUrl removeTaskListUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/users/@me/lists/");
    url.addPath(tasklistID);

    return url;
}

/******************************* PRIVATE ******************************/

ObjectPtr Private::JSONToTaskList(QVariantMap jsonData)
{
    TaskListPtr taskList(new TaskList());

    taskList->setUid(jsonData.value(QLatin1String("id")).toString());
    taskList->setEtag(jsonData.value(QLatin1String("etag")).toString());
    taskList->setTitle(jsonData.value(QLatin1String("title")).toString());

    return taskList.dynamicCast<Object>();
}

TaskListPtr JSONToTaskList(const QByteArray& jsonData)
{
    QJson::Parser parser;

    const QVariantMap data = parser.parse(jsonData).toMap();

    if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#taskList")) {
        return Private::JSONToTaskList(data).staticCast<TaskList>();
    }

    return TaskListPtr();
}

ObjectPtr Private::JSONToTask(QVariantMap jsonData)
{
    TaskPtr task(new Task());

    task->setUid(jsonData.value(QLatin1String("id")).toString());
    task->setEtag(jsonData.value(QLatin1String("etag")).toString());
    task->setSummary(jsonData.value(QLatin1String("title")).toString());;
    task->setLastModified(KDateTime::fromString(jsonData.value(QLatin1String("updated")).toString(), KDateTime::RFC3339Date));
    task->setDescription(jsonData.value(QLatin1String("notes")).toString());

    if (jsonData.value(QLatin1String("status")).toString() == QLatin1String("needsAction")) {
        task->setStatus(KCalCore::Incidence::StatusNeedsAction);
    } else if (jsonData.value(QLatin1String("status")).toString() == QLatin1String("completed")) {
        task->setStatus(KCalCore::Incidence::StatusCompleted);
    } else {
        task->setStatus(KCalCore::Incidence::StatusNone);
    }

    task->setDtDue(KDateTime::fromString(jsonData.value(QLatin1String("due")).toString(), KDateTime::RFC3339Date));

    if (task->status() == KCalCore::Incidence::StatusCompleted) {
        task->setCompleted(KDateTime::fromString(jsonData.value(QLatin1String("completed")).toString(), KDateTime::RFC3339Date));
    }

    task->setDeleted(jsonData.value(QLatin1String("deleted")).toBool());

    if (jsonData.contains(QLatin1String("parent"))) {
        task->setRelatedTo(jsonData.value(QLatin1String("parent")).toString(), KCalCore::Incidence::RelTypeParent);
    }

    return task.dynamicCast<Object>();
}

TaskPtr JSONToTask(const QByteArray& jsonData)
{
    QJson::Parser parser;

    const QVariantMap data = parser.parse(jsonData).toMap();

    if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#task")) {
        return Private::JSONToTask(data).staticCast<Task>();
    }

    return TaskPtr();
}

QByteArray taskListToJSON(const TaskListPtr &taskList)
{
    QVariantMap output;

    output.insert(QLatin1String("kind"), QLatin1String("tasks#taskList"));
    if (!taskList->uid().isEmpty()) {
        output.insert(QLatin1String("id"), taskList->uid());
    }
    output.insert(QLatin1String("title"), taskList->title());

    QJson::Serializer serializer;
    return serializer.serialize(output);
}

QByteArray taskToJSON(const TaskPtr &task)
{
    QVariantMap output;

    output.insert(QLatin1String("kind"), QLatin1String("tasks#task"));

    if (!task->uid().isEmpty()) {
        output.insert(QLatin1String("id"), task->uid());
    }

    output.insert(QLatin1String("title"), task->summary());
    output.insert(QLatin1String("notes"), task->description());

    if (!task->relatedTo(KCalCore::Incidence::RelTypeParent).isEmpty()) {
        output.insert(QLatin1String("parent"), task->relatedTo(KCalCore::Incidence::RelTypeParent));
    }

    if (task->dtDue().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QLatin1String("due"), task->dtDue().toUtc().toString(QLatin1String("%Y-%m-%dT%H:%M:%S.%:sZ")));
    }

    if ((task->status() == KCalCore::Incidence::StatusCompleted) && task->completed().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QLatin1String("completed"), task->completed().toUtc().toString(QLatin1String("%Y-%m-%dT%H:%M:%S.%:sZ")));
        output.insert(QLatin1String("status"), QLatin1String("completed"));
    } else {
        output.insert(QLatin1String("status"), QLatin1String("needsAction"));
    }

    QJson::Serializer serializer;
    return serializer.serialize(output);
}

ObjectsList Private::parseTaskListJSONFeed(const QVariantList &items)
{
    ObjectsList list;

    Q_FOREACH(const QVariant &item, items) {
        list.append(Private::JSONToTaskList(item.toMap()));
    }

    return list;
}

ObjectsList Private::parseTasksJSONFeed(const QVariantList &items)
{
    ObjectsList list;

    Q_FOREACH(const QVariant &item, items) {
        list.append(Private::JSONToTask(item.toMap()));
    }

    return list;
}

} // namespace TasksService

} // namespace KGAPI2
