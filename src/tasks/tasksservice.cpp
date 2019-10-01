/*
    Copyright (C) 2012 - 2018  Daniel Vrátil <dvratil@kde.org>

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
    License along with this library.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "tasksservice.h"
#include "object.h"
#include "task.h"
#include "tasklist.h"
#include "utils.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QVariant>

namespace KGAPI2
{

namespace TasksService
{

    namespace {
        /* Google accepts only UTC time strictly in this format :( */
        static const auto DatetimeFormat = QStringLiteral("yyyy-MM-ddThh:mm:ss.zzzZ");

        static const auto TasksUrlPart = QLatin1String("/tasks");

        static const auto PageTokenParam = QStringLiteral("pageToken");
        static const auto MaxResultsParam = QStringLiteral("maxResults");
        static const auto MaxResultsParamValueDefault = QStringLiteral("20");

        static const auto KindAttr = QStringLiteral("kind");
        static const auto IdAttr = QStringLiteral("id");
        static const auto EtagAttr = QStringLiteral("etag");
        static const auto TitleAttr = QStringLiteral("title");
        static const auto NotesAttr = QStringLiteral("notes");
        static const auto ItemsAttr = QStringLiteral("items");
        static const auto NextPageTokenAttr = QStringLiteral("nextPageToken");
        static const auto ParentAttr = QStringLiteral("parent");
        static const auto SelfLinkAttr = QStringLiteral("selfLink");
        static const auto UpdatedAttr = QStringLiteral("updated");
        static const auto StatusAttr = QStringLiteral("status");
        static const auto DueAttr = QStringLiteral("due");
        static const auto DeletedAttr = QStringLiteral("deleted");

        static const auto CompletedAttrVal = QLatin1String("completed");
        static const auto NeedsActionAttrVal = QLatin1String("needsAction");
    }

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

    if (feed.value(KindAttr).toString() == QLatin1String("tasks#taskLists")) {
        list = Private::parseTaskListJSONFeed(feed.value(ItemsAttr).toList());

        if (feed.contains(NextPageTokenAttr)) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            QUrlQuery query(feedData.nextPageUrl);
            query.addQueryItem(PageTokenParam, feed.value(NextPageTokenAttr).toString());
            if (query.queryItemValue(MaxResultsParam).isEmpty()) {
                query.addQueryItem(MaxResultsParam, MaxResultsParamValueDefault);
            }
            feedData.nextPageUrl.setQuery(query);
        }

    } else if (feed.value(KindAttr).toString() == QLatin1String("tasks#tasks")) {
        list = Private::parseTasksJSONFeed(feed.value(ItemsAttr).toList());

        if (feed.contains(NextPageTokenAttr)) {
            QString taskListId = feedData.requestUrl.toString().remove(QStringLiteral("https://www.googleapis.com/tasks/v1/lists/"));
            taskListId = taskListId.left(taskListId.indexOf(QLatin1Char('/')));

            feedData.nextPageUrl = fetchAllTasksUrl(taskListId);
            QUrlQuery query(feedData.nextPageUrl);
            query.addQueryItem(PageTokenParam, feed.value(NextPageTokenAttr).toString());
            if (query.queryItemValue(MaxResultsParam).isEmpty()) {
                query.addQueryItem(MaxResultsParam, MaxResultsParamValueDefault);
            }
            feedData.nextPageUrl.setQuery(query);
        }
    }

    return list;
}

QUrl fetchAllTasksUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart);
    return url;
}

QUrl fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart % QLatin1Char('/') % taskID);
    return url;
}

QUrl createTaskUrl(const QString& tasklistID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart);
    return url;
}

QUrl updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart % QLatin1Char('/') % taskID);
    return url;
}

QUrl removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart % QLatin1Char('/') % taskID);
    return url;
}

QUrl moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
{
    QUrl url(Private::GoogleApisUrl);
    url.setPath(Private::TasksBasePath % QLatin1Char('/') % tasklistID % TasksUrlPart % QLatin1Char('/') % taskID % QLatin1String("/move"));
    if (!newParent.isEmpty()) {
        QUrlQuery query(url);
        query.addQueryItem(ParentAttr, newParent);
        url.setQuery(query);
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

    taskList->setUid(jsonData.value(IdAttr).toString());
    taskList->setEtag(jsonData.value(EtagAttr).toString());
    taskList->setTitle(jsonData.value(TitleAttr).toString());
    taskList->setSelfLink(jsonData.value(SelfLinkAttr).toString());
    taskList->setUpdated(jsonData.value(UpdatedAttr).toString());

    return taskList.dynamicCast<Object>();
}

TaskListPtr JSONToTaskList(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();

    if (data.value(KindAttr).toString() == QLatin1String("tasks#taskList")) {
        return Private::JSONToTaskList(data).staticCast<TaskList>();
    }

    return TaskListPtr();
}

ObjectPtr Private::JSONToTask(const QVariantMap &jsonData)
{
    TaskPtr task(new Task());

    task->setUid(jsonData.value(IdAttr).toString());
    task->setEtag(jsonData.value(EtagAttr).toString());
    task->setSummary(jsonData.value(TitleAttr).toString());
    task->setLastModified(Utils::rfc3339DateFromString(jsonData.value(UpdatedAttr).toString()));
    task->setDescription(jsonData.value(NotesAttr).toString());

    if (jsonData.value(StatusAttr).toString() == NeedsActionAttrVal) {
        task->setStatus(KCalendarCore::Incidence::StatusNeedsAction);
    } else if (jsonData.value(StatusAttr).toString() == CompletedAttrVal) {
        task->setStatus(KCalendarCore::Incidence::StatusCompleted);
    } else {
        task->setStatus(KCalendarCore::Incidence::StatusNone);
    }

    task->setDtDue(Utils::rfc3339DateFromString(jsonData.value(DueAttr).toString()));

    if (task->status() == KCalendarCore::Incidence::StatusCompleted) {
        task->setCompleted(Utils::rfc3339DateFromString(jsonData.value(CompletedAttrVal).toString()));
    }

    task->setDeleted(jsonData.value(DeletedAttr).toBool());

    if (jsonData.contains(ParentAttr)) {
        task->setRelatedTo(jsonData.value(ParentAttr).toString(), KCalendarCore::Incidence::RelTypeParent);
    }

    return task.dynamicCast<Object>();
}

TaskPtr JSONToTask(const QByteArray& jsonData)
{
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    const QVariantMap data = document.toVariant().toMap();

    if (data.value(KindAttr).toString() == QLatin1String("tasks#task")) {
        return Private::JSONToTask(data).staticCast<Task>();
    }

    return TaskPtr();
}

QByteArray taskListToJSON(const TaskListPtr &taskList)
{
    QVariantMap output;

    output.insert(KindAttr, QStringLiteral("tasks#taskList"));
    if (!taskList->uid().isEmpty()) {
        output.insert(IdAttr, taskList->uid());
    }
    output.insert(TitleAttr, taskList->title());

    QJsonDocument document = QJsonDocument::fromVariant(output);
    return document.toJson(QJsonDocument::Compact);
}

QByteArray taskToJSON(const TaskPtr &task)
{
    QVariantMap output;

    output.insert(KindAttr, QStringLiteral("tasks#task"));

    if (!task->uid().isEmpty()) {
        output.insert(IdAttr, task->uid());
    }

    output.insert(TitleAttr, task->summary());
    output.insert(NotesAttr, task->description());

    if (!task->relatedTo(KCalendarCore::Incidence::RelTypeParent).isEmpty()) {
        output.insert(ParentAttr, task->relatedTo(KCalendarCore::Incidence::RelTypeParent));
    }

    if (task->dtDue().isValid()) {
        output.insert(DueAttr, task->dtDue().toUTC().toString(DatetimeFormat));
    }

    if ((task->status() == KCalendarCore::Incidence::StatusCompleted) && task->completed().isValid()) {
        output.insert(CompletedAttrVal, task->completed().toUTC().toString(DatetimeFormat));
        output.insert(StatusAttr, CompletedAttrVal);
    } else {
        output.insert(StatusAttr, NeedsActionAttrVal);
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
