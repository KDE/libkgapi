/*
    libKGoogle - Tasks Service
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
#include "kgoogleobject.h"
#include "objects/task.h"
#include "objects/tasklist.h"
#include "kgoogleaccessmanager.h"

#include "qjson/parser.h"
#include "qjson/serializer.h"

using namespace KGoogle;
using namespace Service;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

Tasks::~Tasks()
{

}

QByteArray Tasks::objectToXML(KGoogleObject* object)
{
  Q_UNUSED (object);
  
  return QByteArray();
}

KGoogleObject* Tasks::XMLToObject(const QByteArray& xmlData)
{
  Q_UNUSED (xmlData);
  
  return 0;
}

QList< KGoogleObject* > Tasks::parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData)
{
  Q_UNUSED (xmlFeed);
  Q_UNUSED (feedData);
  
  return QList< KGoogleObject* >();
}

QList< KGoogleObject* > Tasks::parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData)
{
  QJson::Parser parser;
  
  QList< KGoogleObject* > list;
  QVariantMap feed = parser.parse(jsonFeed).toMap();

  if (feed["kind"] == "tasks#taskLists") {
    list = parseTaskListJSONFeed(feed["items"].toList());
  } else if (feed["kind"] == "tasks#tasks") {
    list = parseTasksJSONFeed(feed["items"].toList());
  }

  if (feedData) {
    feedData->itemsPerPage = 0;
    feedData->startIndex = 0;
    feedData->totalResults = 0;
    if (feed.contains("nextPageToken"))
      feedData->nextLink = QUrl(fetchTaskListsUrl() + "?pageToken=" + feed["nextPageToken"].toString());
  }

  return list;
}

KGoogleObject* Tasks::JSONToObject(const QByteArray& jsonData)
{
  QJson::Parser parser;
  KGoogleObject *object;
  
  QVariantMap data = parser.parse(jsonData).toMap();
  
  if (data["kind"] == "tasks#taskList") {
    object = JSONToTaskList(data);
  } else if (data["kind"] == "tasks#task") {
    object = JSONToTask(data);
  }
  
  return object;
}

QByteArray Tasks::objectToJSON(KGoogleObject* object)
{
  QVariantMap map;
  
  if (dynamic_cast<const Object::TaskList*>(object)) {
    map = taskListToJSON(object);
  } else if (dynamic_cast<const Object::Task*>(object)) {
    map = taskToJSON(object);
  }
  
  QJson::Serializer serializer;
  return serializer.serialize(map);  
}

QString Tasks::scopeUrl()
{
  return "https://www.googleapis.com/auth/tasks";
}

QString Tasks::fetchAllTasksUrl()
{
  // %1 = taskList
  return "https://www.googleapis.com/tasks/v1/lists/%1/tasks";
}

QString Tasks::fetchTaskUrl()
{
  // %1 = taskList
  // %2 = task ID
  return "https://www.googleapis.com/tasks/v1/lists/%1/tasks/%2";
}

QString Tasks::createTaskUrl()
{
  // %1 = taskList
  return "https://www.googleapis.com/tasks/v1/lists/%1/tasks";
}

QString Tasks::updateTaskUrl()
{
  // %1 = taskList
  // %2 = task ID
  return "https://www.googleapis.com/tasks/v1/lists/%1/tasks/%2";
}

QString Tasks::removeTaskUrl()
{
  // %1 = taskList
  // %2 = task ID
  return "https://www.googleapis.com/tasks/v1/lists/%1/tasks/%2";
}

QString Tasks::fetchTaskListsUrl()
{
  return "https://www.googleapis.com/tasks/v1/users/@me/lists";
}

QString Tasks::moveTaskUrl()
{
  // %1 = taskList
  // %2 = task ID
  return "https://www.googleapis.com//tasks/v1/lists/%1/tasks/%2/move";
}


const QString Tasks::protocolVersion()
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

KGoogleObject* Tasks::JSONToTaskList(QVariantMap jsonData)
{
  Object::TaskList *object = new Object::TaskList();

  object->setId(jsonData["id"].toString());
  object->setEtag(jsonData["etag"].toString());
  object->setTitle(jsonData["title"].toString());
  
  return dynamic_cast< KGoogleObject* >(object);
}

KGoogleObject* Tasks::JSONToTask(QVariantMap jsonData)
{
  Object::Task *object = new Object::Task();
  
  object->setId(jsonData["id"].toString());
  object->setEtag(jsonData["etag"].toString());
  object->setSummary(jsonData["title"].toString());;
  object->setLastModified(KGoogleAccessManager::RFC3339StringToDate(jsonData["updated"].toString()));
  object->setDescription(jsonData["notes"].toString());
  
  if (jsonData["status"].toString() == "needsAction")
    object->setStatus(Incidence::StatusNeedsAction);
  else if (jsonData["status"].toString() == "completed")
    object->setStatus(Incidence::StatusCompleted);
  else 
    object->setStatus(Incidence::StatusNone);
  
  object->setDtDue(KGoogleAccessManager::RFC3339StringToDate(jsonData["due"].toString()));
  
  if (object->status() == Incidence::StatusCompleted)
    object->setCompleted(KGoogleAccessManager::RFC3339StringToDate(jsonData["completed"].toString()));
  
  object->setDeleted(jsonData["deleted"].toBool());
  
  if (jsonData.contains("parent")) {
#ifdef WITH_KCAL
    object->setRelatedToUid(jsonData["parent"].toString());
#else
    object->setRelatedTo(jsonData["parent"].toString(), Incidence::RelTypeParent);
#endif
  }

  
  return dynamic_cast< KGoogleObject* >(object);
}

QVariantMap Tasks::taskListToJSON(KGoogleObject* taskList)
{
  QVariantMap output;
  Object::TaskList *object = static_cast< Object::TaskList* >(taskList);
  
  output["id"] = object->id();
  output["title"] = object->title();

  return output;
}

QVariantMap Tasks::taskToJSON(KGoogleObject* task)
{
  QVariantMap output;
  Object::Task *object = static_cast< Object::Task* >(task);
  
  if (!object->id().isEmpty())
    output["id"] = object->id();
  output["title"] = object->summary();
  output["notes"] = object->description();
  
#ifdef WITH_KCAL
  if (!object->relatedToUid().isEmpty())
    output["parent"] = object->relatedToUid();
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

QList< KGoogleObject* > Tasks::parseTaskListJSONFeed(QVariantList items)
{
  QList< KGoogleObject* > list;
  
  foreach (QVariant item, items) {
    list.append(JSONToTaskList(item.toMap()));
  }
  
  return list;
}

QList< KGoogleObject* > Tasks::parseTasksJSONFeed(QVariantList items)
{
  QList< KGoogleObject* > list;
  
  foreach (QVariant item, items) {
    list.append(JSONToTask(item.toMap()));
  }
  
  return list;
}





