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

#include "tasks.h"
#include "object.h"
#include "objects/task.h"
#include "objects/tasklist.h"
#include "accessmanager.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <KUrl>
#include <QtCore/QVariant>

using namespace KGAPI;
using namespace Services;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

namespace KGAPI
{

namespace Services
{

class TasksPrivate
{
  public:
    static QList< KGAPI::Object* > parseTaskListJSONFeed(const QVariantList &items);
    static QList< KGAPI::Object* > parseTasksJSONFeed(const QVariantList &items);

    static KGAPI::Object* JSONToTaskList(QVariantMap jsonData);
    static QVariantMap taskListToJSON(KGAPI::Object *taskList);

    static KGAPI::Object* JSONToTask(QVariantMap jsonData);
    static QVariantMap taskToJSON(KGAPI::Object *task);
};

}

}

QUrl Tasks::ScopeUrl(QLatin1String("https://www.googleapis.com/auth/tasks"));

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Tasks");


const QString& Tasks::serviceName()
{
    if (QMetaType::type(serviceNameStr.toLatin1().constData()) == 0) {
        qRegisterMetaType< KGAPI::Services::Tasks >(serviceNameStr.toLatin1().constData());
    }

    return serviceNameStr;
}


QByteArray Tasks::objectToXML(KGAPI::Object* object)
{
    Q_UNUSED(object);

    return QByteArray();
}

KGAPI::Object* Tasks::XMLToObject(const QByteArray& xmlData)
{
    Q_UNUSED(xmlData);

    return 0;
}

QList< KGAPI::Object* > Tasks::parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData)
{
    Q_UNUSED(xmlFeed);
    Q_UNUSED(feedData);

    return QList< KGAPI::Object* >();
}

QList< KGAPI::Object* > Tasks::parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData)
{
    QJson::Parser parser;

    QList< KGAPI::Object* > list;
    const QVariantMap feed = parser.parse(jsonFeed).toMap();

    if (feed.value(QLatin1String("kind")).toString() == QLatin1String("tasks#taskLists")) {
        list = TasksPrivate::parseTaskListJSONFeed(feed.value(QLatin1String("items")).toList());

        if (feed.contains(QLatin1String("nextPageToken"))) {
            feedData.nextPageUrl = fetchTaskListsUrl();
            feedData.nextPageUrl.addQueryItem(QLatin1String("pageToken"), feed.value(QLatin1String("nextPageToken")).toString());
            if (feedData.nextPageUrl.queryItemValue(QLatin1String("maxResults")).isEmpty()) {
                feedData.nextPageUrl.addQueryItem(QLatin1String("maxResults"), QLatin1String("20"));
            }
        }

    } else if (feed.value(QLatin1String("kind")).toString() == QLatin1String("tasks#tasks")) {
        list = TasksPrivate::parseTasksJSONFeed(feed.value(QLatin1String("items")).toList());

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

KGAPI::Object* Tasks::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;
    KGAPI::Object *object = 0;

    const QVariantMap data = parser.parse(jsonData).toMap();

    if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#taskList")) {
        object = TasksPrivate::JSONToTaskList(data);
    } else if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#task")) {
        object = TasksPrivate::JSONToTask(data);
    }

    return object;
}

QByteArray Tasks::objectToJSON(KGAPI::Object* object)
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
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));

    return url;
}

QUrl Tasks::fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl Tasks::createTaskUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));

    return url;
}

QUrl Tasks::updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl Tasks::removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/lists/");
    url.addPath(tasklistID);
    url.addPath(QLatin1String("tasks"));
    url.addPath(taskID);

    return url;
}

QUrl Tasks::moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
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

QUrl Tasks::fetchTaskListsUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/tasks/v1/users/@me/lists"));
}

QUrl Tasks::createTaskListUrl()
{
    return QUrl(QLatin1String("https://www.googleapis.com/tasks/v1/users/@me/lists"));
}

QUrl Tasks::updateTaskListUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/users/@me/lists/");
    url.addPath(tasklistID);

    return url;
}

QUrl Tasks::removeTaskListUrl(const QString& tasklistID)
{
    KUrl url("https://www.googleapis.com/tasks/v1/users/@me/lists/");
    url.addPath(tasklistID);

    return url;
}

QString Tasks::protocolVersion() const
{
    return QLatin1String("");
}

bool Tasks::supportsJSONRead(QString* urlParam)
{
    if (urlParam) {
        urlParam->clear();
    }

    return true;
}

bool Tasks::supportsJSONWrite(QString* urlParam)
{
    if (urlParam) {
        urlParam->clear();
    }

    return true;
}

/******************************* PRIVATE ******************************/

KGAPI::Object* TasksPrivate::JSONToTaskList(QVariantMap jsonData)
{
    Objects::TaskList *object = new Objects::TaskList();

    object->setUid(jsonData.value(QLatin1String("id")).toString());
    object->setEtag(jsonData.value(QLatin1String("etag")).toString());
    object->setTitle(jsonData.value(QLatin1String("title")).toString());

    return dynamic_cast< KGAPI::Object* >(object);
}

KGAPI::Object* TasksPrivate::JSONToTask(QVariantMap jsonData)
{
    Objects::Task *object = new Objects::Task();

    object->setUid(jsonData.value(QLatin1String("id")).toString());
    object->setEtag(jsonData.value(QLatin1String("etag")).toString());
    object->setSummary(jsonData.value(QLatin1String("title")).toString());;
    object->setLastModified(AccessManager::RFC3339StringToDate(jsonData.value(QLatin1String("updated")).toString()));
    object->setDescription(jsonData.value(QLatin1String("notes")).toString());

    if (jsonData.value(QLatin1String("status")).toString() == QLatin1String("needsAction")) {
        object->setStatus(Incidence::StatusNeedsAction);
    } else if (jsonData.value(QLatin1String("status")).toString() == QLatin1String("completed")) {
        object->setStatus(Incidence::StatusCompleted);
    } else {
        object->setStatus(Incidence::StatusNone);
    }

    object->setDtDue(AccessManager::RFC3339StringToDate(jsonData.value(QLatin1String("due")).toString()));

    if (object->status() == Incidence::StatusCompleted) {
        object->setCompleted(AccessManager::RFC3339StringToDate(jsonData.value(QLatin1String("completed")).toString()));
    }

    object->setDeleted(jsonData.value(QLatin1String("deleted")).toBool());

    if (jsonData.contains(QLatin1String("parent"))) {
#ifdef WITH_KCAL
        object->setRelatedToUid(jsonData.value(QLatin1String("parent")).toString());
#else
        object->setRelatedTo(jsonData.value(QLatin1String("parent")).toString(), Incidence::RelTypeParent);
#endif
    }

    return dynamic_cast< KGAPI::Object* >(object);
}

QVariantMap TasksPrivate::taskListToJSON(KGAPI::Object* taskList)
{
    QVariantMap output;
    Objects::TaskList *object = static_cast< Objects::TaskList* >(taskList);

    output.insert(QLatin1String("kind"), QLatin1String("tasks#taskList"));
    if (!object->uid().isEmpty()) {
        output.insert(QLatin1String("id"), object->uid());
    }
    output.insert(QLatin1String("title"), object->title());

    return output;
}

QVariantMap TasksPrivate::taskToJSON(KGAPI::Object* task)
{
    QVariantMap output;
    Objects::Task *object = static_cast< Objects::Task* >(task);

    output.insert(QLatin1String("kind"), QLatin1String("tasks#task"));

    if (!object->uid().isEmpty()) {
        output.insert(QLatin1String("id"), object->uid());
    }

    output.insert(QLatin1String("title"), object->summary());
    output.insert(QLatin1String("notes"), object->description());

#ifdef WITH_KCAL
    if (!object->relatedToUid().isEmpty()) {
        output.insert(QLatin1String("parent"), object->relatedToUid());
    }
#else
    if (!object->relatedTo(Incidence::RelTypeParent).isEmpty()) {
        output.insert(QLatin1String("parent"), object->relatedTo(Incidence::RelTypeParent));
    }
#endif

    if (object->dtDue().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QLatin1String("due"), object->dtDue().toUtc().toString(QLatin1String("%Y-%m-%dT%H:%M:%S.%:sZ")));
    }

    if ((object->status() == Incidence::StatusCompleted) && object->completed().isValid()) {
        /* Google accepts only UTC time strictly in this format :( */
        output.insert(QLatin1String("completed"), object->completed().toUtc().toString(QLatin1String("%Y-%m-%dT%H:%M:%S.%:sZ")));
        output.insert(QLatin1String("status"), QLatin1String("completed"));
    } else {
        output.insert(QLatin1String("status"), QLatin1String("needsAction"));
    }

    return output;
}

QList< KGAPI::Object* > TasksPrivate::parseTaskListJSONFeed(const QVariantList &items)
{
    QList< KGAPI::Object* > list;

    Q_FOREACH(const QVariant &item, items) {
        list.append(JSONToTaskList(item.toMap()));
    }

    return list;
}

QList< KGAPI::Object* > TasksPrivate::parseTasksJSONFeed(const QVariantList &items)
{
    QList< KGAPI::Object* > list;

    Q_FOREACH(const QVariant &item, items) {
        list.append(JSONToTask(item.toMap()));
    }

    return list;
}
