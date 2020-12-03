/*
    SPDX-FileCopyrightText: 2012-2018 Daniel Vrátil <dvratil@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tasksservice.h"
#include "object.h"
#include "task.h"
#include "tasklist.h"
#include "utils.h"
#include "propertymapper_p.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QVariant>

#include <tuple>

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
    ObjectsList parseTaskListJSONFeed(const QJsonArray &items);
    ObjectsList parseTasksJSONFeed(const QJsonArray &items);

    ObjectPtr JSONToTaskList(const QJsonValue &jsonData);
    ObjectPtr JSONToTask(const QJsonValue &jsonData);

    static const QUrl GoogleApisUrl(QStringLiteral("https://www.googleapis.com"));
    static const QString TasksBasePath(QStringLiteral("/tasks/v1/lists"));
    static const QString TasksListsBasePath(QStringLiteral("/tasks/v1/users/@me/lists"));
} // namespace Private

} // namespace TasksService

template<>
KCalendarCore::Incidence::Status convertTo<KCalendarCore::Incidence::Status>(const QJsonValue &val) {
    if (val == TasksService::NeedsActionAttrVal) {
        return KCalendarCore::Incidence::StatusNeedsAction;
    } else if (val == TasksService::CompletedAttrVal) {
        return KCalendarCore::Incidence::StatusCompleted;
    } else {
        return KCalendarCore::Incidence::StatusNone;
    }
}

namespace TasksService
{

ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    const auto document = QJsonDocument::fromJson(jsonFeed);
    if (document.isNull()) {
        return ObjectsList();
    }

    ObjectsList list;
    const auto feed = document.object();

    if (feed[KindAttr].toString() == QLatin1String("tasks#taskLists")) {
        list = Private::parseTaskListJSONFeed(feed[ItemsAttr].toArray());

        if (feed.contains(NextPageTokenAttr)) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            QUrlQuery query(feedData.nextPageUrl);
            query.addQueryItem(PageTokenParam, feed[NextPageTokenAttr].toString());
            if (query.queryItemValue(MaxResultsParam).isEmpty()) {
                query.addQueryItem(MaxResultsParam, MaxResultsParamValueDefault);
            }
            feedData.nextPageUrl.setQuery(query);
        }

    } else if (feed[KindAttr].toString() == QLatin1String("tasks#tasks")) {
        list = Private::parseTasksJSONFeed(feed[ItemsAttr].toArray());

        if (feed.contains(NextPageTokenAttr)) {
            QString taskListId = feedData.requestUrl.toString().remove(QStringLiteral("https://www.googleapis.com/tasks/v1/lists/"));
            taskListId = taskListId.left(taskListId.indexOf(QLatin1Char('/')));

            feedData.nextPageUrl = fetchAllTasksUrl(taskListId);
            QUrlQuery query(feedData.nextPageUrl);
            query.addQueryItem(PageTokenParam, feed[NextPageTokenAttr].toString());
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

ObjectPtr Private::JSONToTaskList(const QJsonValue &jsonData)
{
    TaskListPtr taskList(new TaskList());

    PropertyMapper{taskList.get(), jsonData.toObject()}(
         Mapping{&TaskList::setUid, IdAttr},
         Mapping{&TaskList::setEtag, EtagAttr},
         Mapping{&TaskList::setTitle, TitleAttr},
         Mapping{&TaskList::setSelfLink, SelfLinkAttr},
         Mapping{&TaskList::setUpdated, UpdatedAttr}
    );

    return taskList.dynamicCast<Object>();
}

TaskListPtr JSONToTaskList(const QByteArray& jsonData)
{
    const auto document = QJsonDocument::fromJson(jsonData);

    if (document[KindAttr].toString() == QLatin1String("tasks#taskList")) {
        return Private::JSONToTaskList(document.object()).staticCast<TaskList>();
    }

    return TaskListPtr();
}

ObjectPtr Private::JSONToTask(const QJsonValue &json)
{
    const QJsonObject jsonData = json.toObject();

    TaskPtr task(new Task());

    PropertyMapper mapper{task.get(), jsonData};
    mapper(
        Mapping{&Task::setUid, IdAttr},
        Mapping{&Task::setEtag, EtagAttr},
        Mapping{qOverload<const QString &>(&Task::setSummary), TitleAttr},
        Mapping{qOverload<const QString &>(&Task::setLocation), UpdatedAttr},
        Mapping{qOverload<const QString &>(&Task::setDescription), NotesAttr},
        Mapping{&Task::setStatus, StatusAttr},
        Mapping{&Task::setDtDue, DueAttr, false},
        Mapping{&Task::setDeleted, DeletedAttr}
    );

    if (task->status() == KCalendarCore::Incidence::StatusCompleted) {
        mapper(qOverload<const QDateTime &>(&Task::setCompleted), CompletedAttrVal);
    }
    if (jsonData.contains(ParentAttr)) {
        mapper(&Task::setRelatedTo, ParentAttr, KCalendarCore::Incidence::RelTypeParent);
    }

    return task.dynamicCast<Object>();
}

TaskPtr JSONToTask(const QByteArray& jsonData)
{
    const auto document = QJsonDocument::fromJson(jsonData);

    if (document[KindAttr].toString() == QLatin1String("tasks#task")) {
        return Private::JSONToTask(document.object()).staticCast<Task>();
    }

    return TaskPtr();
}

QByteArray taskListToJSON(const TaskListPtr &taskList)
{
    QJsonObject output;
    output[KindAttr] = QStringLiteral("tasks#taskList");
    if (!taskList->uid().isEmpty()) {
        output[IdAttr] = taskList->uid();
    }
    output[TitleAttr] = taskList->title();

    return QJsonDocument{output}.toJson(QJsonDocument::Compact);
}

QByteArray taskToJSON(const TaskPtr &task)
{
    QJsonObject output;
    output[KindAttr] = QStringLiteral("tasks#task");

    if (!task->uid().isEmpty()) {
        output[IdAttr] = task->uid();
    }

    output[TitleAttr] = task->summary();
    output[NotesAttr] = task->description();

    if (!task->relatedTo(KCalendarCore::Incidence::RelTypeParent).isEmpty()) {
        output[ParentAttr] = task->relatedTo(KCalendarCore::Incidence::RelTypeParent);
    }

    if (task->dtDue().isValid()) {
        output[DueAttr] = task->dtDue().toUTC().toString(DatetimeFormat);
    }

    if ((task->status() == KCalendarCore::Incidence::StatusCompleted) && task->completed().isValid()) {
        output[CompletedAttrVal] = task->completed().toUTC().toString(DatetimeFormat);
        output[StatusAttr] = CompletedAttrVal;
    } else {
        output[StatusAttr] = NeedsActionAttrVal;
    }

    return QJsonDocument{output}.toJson(QJsonDocument::Compact);
}

ObjectsList Private::parseTaskListJSONFeed(const QJsonArray &items)
{
    ObjectsList list;
    list.reserve(items.size());
    std::transform(items.cbegin(), items.cend(), std::back_inserter(list), Private::JSONToTaskList);
    return list;
}

ObjectsList Private::parseTasksJSONFeed(const QJsonArray &items)
{
    ObjectsList list;
    list.reserve(items.size());
    std::transform(items.cbegin(), items.cend(), std::back_inserter(list), Private::JSONToTask);
    return list;
}

} // namespace TasksService

} // namespace KGAPI2
