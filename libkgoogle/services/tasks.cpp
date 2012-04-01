/*
    libKGoogle - Services - Tasks
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "tasks.h"
#include "object.h"
#include "objects/task.h"
#include "objects/tasklist.h"
#include "accessmanager.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QtCore/QVariant>

using namespace KGoogle;
using namespace Services;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

namespace KGoogle
{

namespace Services
{

class TasksPrivate
{
  public:
    static QList< KGoogle::Object* > parseTaskListJSONFeed(const QVariantList &items);
    static QList< KGoogle::Object* > parseTasksJSONFeed(const QVariantList &items);

    static KGoogle::Object* JSONToTaskList(QVariantMap jsonData);
    static QVariantMap taskListToJSON(KGoogle::Object *taskList);

    static KGoogle::Object* JSONToTask(QVariantMap jsonData);
    static QVariantMap taskToJSON(KGoogle::Object *task);
};

}

}

QUrl Tasks::ScopeUrl("https://www.googleapis.com/auth/tasks");

QByteArray Tasks::objectToXML(KGoogle::Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
}

KGoogle::Object* Tasks::XMLToObject(const QByteArray& xmlData)
{
    Q_UNUSED(xmlData);

    return 0;
}

QList< KGoogle::Object* > Tasks::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGoogle::Object* >();
}

QList< KGoogle::Object* > Tasks::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QList< KGoogle::Object* > list;
    QVariantMap feed = parser.parse(jsonFeed).toMap();

    if (feed["kind"].toString() == "tasks#taskLists") {
        list = TasksPrivate::parseTaskListJSONFeed(feed["items"].toList());

        if (feed.contains("nextPageToken")) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            feedData.nextPageUrl.addQueryItem("pageToken", feed["nextPageToken"].toString());
            if (feedData.nextPageUrl.queryItemValue("maxResults").isEmpty()) {
                feedData.nextPageUrl.addQueryItem("maxResults", "20");
            }
        }

    } else if (feed["kind"].toString() == "tasks#tasks") {
        list = TasksPrivate::parseTasksJSONFeed(feed["items"].toList());

        if (feed.contains("nextPageToken")) {
            QString taskListId = feedData.requestUrl.toString().remove("https://www.googleapis.com/tasks/v1/lists/");
            taskListId = taskListId.left(taskListId.indexOf("/"));

            feedData.nextPageUrl = fetchAllTasksUrl(taskListId);
            feedData.nextPageUrl.addQueryItem("pageToken", feed["nextPageToken"].toString());
            if (feedData.nextPageUrl.queryItemValue("maxResults").isEmpty()) {
                feedData.nextPageUrl.addQueryItem("maxResults", "20");
            }
        }
    }


    return list;
}

KGoogle::Object* Tasks::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;
    KGoogle::Object *object = 0;

    QVariantMap data = parser.parse(jsonData).toMap();

    if (data["kind"].toString() == "tasks#taskList") {
        object = TasksPrivate::JSONToTaskList(data);
    } else if (data["kind"].toString() == "tasks#task") {
        object = TasksPrivate::JSONToTask(data);
    }

    return object;
}

QByteArray Tasks::objectToJSON(KGoogle::Object* object)
{
    QVariantMap map;

    if (dynamic_cast< const Objects::TaskList* >(object)) {
        map = TasksPrivate::taskListToJSON(object);
    } else if (dynamic_cast< const Objects::Task* >(object)) {
        map = TasksPrivate::taskToJSON(object);
    }

    QJson::Serializer serializer;
    return serializer.serialize(map);
}

const QUrl& Tasks::scopeUrl() const
{
    return Tasks::ScopeUrl;
}

QUrl Tasks::fetchAllTasksUrl(const QString& tasklistID)
{
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks";
}

QUrl Tasks::fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks/" + taskID;
}

QUrl Tasks::createTaskUrl(const QString& tasklistID)
{
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks";
}

QUrl Tasks::updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks/" + taskID;
}

QUrl Tasks::removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks/" + taskID;
}

QUrl Tasks::moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
{
    QString parent = (!newParent.isEmpty()) ? "?parent=" + newParent : "";
    return "https://www.googleapis.com/tasks/v1/lists/" + tasklistID + "/tasks/" + taskID + "/move" + parent;
}

QUrl Tasks::fetchTaskListsUrl()
{
    return QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists");
}

QUrl Tasks::createTaskListUrl()
{
    return QUrl("https://www.googleapis.com/tasks/v1/users/@me/lists");
}

QUrl Tasks::updateTaskListUrl(const QString& tasklistID)
{
    return "https://www.googleapis.com/tasks/v1/users/@me/lists/" + tasklistID;
}

QUrl Tasks::removeTaskListUrl(const QString& tasklistID)
{
    return "https://www.googleapis.com/tasks/v1/users/@me/lists/" + tasklistID;
}

QString Tasks::protocolVersion() const
{
    return "";
}

bool Tasks::supportsJSONRead(QString* urlParam)
{
    if (urlParam)
        *urlParam = QString();

    return true;
}

bool Tasks::supportsJSONWrite(QString* urlParam)
{
    if (urlParam)
        *urlParam = QString();

    return true;
}

/******************************* PRIVATE ******************************/

KGoogle::Object* TasksPrivate::JSONToTaskList(QVariantMap jsonData)
{
    Objects::TaskList *object = new Objects::TaskList();

    object->setUid(jsonData["id"].toString());
    object->setEtag(jsonData["etag"].toString());
    object->setTitle(jsonData["title"].toString());

    return dynamic_cast< KGoogle::Object* >(object);
}

KGoogle::Object* TasksPrivate::JSONToTask(QVariantMap jsonData)
{
    Objects::Task *object = new Objects::Task();

    object->setUid(jsonData["id"].toString());
    object->setEtag(jsonData["etag"].toString());
    object->setSummary(jsonData["title"].toString());;
    object->setLastModified(AccessManager::RFC3339StringToDate(jsonData["updated"].toString()));
    object->setDescription(jsonData["notes"].toString());

    if (jsonData["status"].toString() == "needsAction") {
        object->setStatus(Incidence::StatusNeedsAction);
    } else if (jsonData["status"].toString() == "completed") {
        object->setStatus(Incidence::StatusCompleted);
    } else {
        object->setStatus(Incidence::StatusNone);
    }

    object->setDtDue(AccessManager::RFC3339StringToDate(jsonData["due"].toString()));

    if (object->status() == Incidence::StatusCompleted) {
        object->setCompleted(AccessManager::RFC3339StringToDate(jsonData["completed"].toString()));
    }

    object->setDeleted(jsonData["deleted"].toBool());

    if (jsonData.contains("parent")) {
#ifdef WITH_KCAL
        object->setRelatedToUid(jsonData["parent"].toString());
#else
        object->setRelatedTo(jsonData["parent"].toString(), Incidence::RelTypeParent);
#endif
    }

    return dynamic_cast< KGoogle::Object* >(object);
}

QVariantMap TasksPrivate::taskListToJSON(KGoogle::Object* taskList)
{
    QVariantMap output;
    Objects::TaskList *object = static_cast< Objects::TaskList* >(taskList);

    output["kind"] = "tasks#taskList";
    if (!object->uid().isEmpty()) {
        output["id"] = object->uid();
    }
    output["title"] = object->title();

    return output;
}

QVariantMap TasksPrivate::taskToJSON(KGoogle::Object* task)
{
    QVariantMap output;
    Objects::Task *object = static_cast< Objects::Task* >(task);

    output["kind"] = "tasks#task";

    if (!object->uid().isEmpty()) {
        output["id"] = object->uid();
    }

    output["title"] = object->summary();
    output["notes"] = object->description();

#ifdef WITH_KCAL
    if (!object->relatedToUid().isEmpty()) {
        output["parent"] = object->relatedToUid();
    }
#else
    if (!object->relatedTo(Incidence::RelTypeParent).isEmpty()) {
        output["parent"] = object->relatedTo(Incidence::RelTypeParent);
    }
#endif

    if (object->dtDue().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output["due"] = object->dtDue().toUtc().toString("%Y-%m-%dT%H:%M:%S.%:sZ");
    }

    if ((object->status() == Incidence::StatusCompleted) && object->completed().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output["completed"] = object->completed().toUtc().toString("%Y-%m-%dT%H:%M:%S.%:sZ");
        output["status"] = "completed";
    } else {
        output["status"] = "needsAction";
    }

    return output;
}

QList< KGoogle::Object* > TasksPrivate::parseTaskListJSONFeed(const QVariantList &items)
{
    QList< KGoogle::Object* > list;

    foreach(QVariant item, items) {
        list.append(JSONToTaskList(item.toMap()));
    }

    return list;
}

QList< KGoogle::Object* > TasksPrivate::parseTasksJSONFeed(const QVariantList &items)
{
    QList< KGoogle::Object* > list;

    foreach(QVariant item, items) {
        list.append(JSONToTask(item.toMap()));
    }

    return list;
}





