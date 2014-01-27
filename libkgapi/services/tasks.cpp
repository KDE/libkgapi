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

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QtCore/QVariant>

#include <libkgapi2/tasks/tasksservice.h>
#include <libkgapi2/tasks/task.h>
#include <libkgapi2/tasks/tasklist.h>
#include <libkgapi2/account.h>

using namespace KGAPI;
using namespace Services;

using namespace KCalCore;

QUrl Tasks::ScopeUrl(KGAPI2::Account::tasksScopeUrl());

static const QString serviceNameStr = QLatin1String("KGAPI::Services::Tasks");


QString Tasks::serviceName()
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
    QList< KGAPI::Object* > list;
    KGAPI2::ObjectsList objects = KGAPI2::TasksService::parseJSONFeed(jsonFeed, feedData);
    if (objects.isEmpty()) {
        return list;
    }

    bool isList = (objects.first().dynamicCast<KGAPI2::TaskList>() != 0);
    Q_FOREACH(const KGAPI2::ObjectPtr &object, objects) {
        if (isList) {
            KGAPI2::TaskListPtr taskList = object.dynamicCast<KGAPI2::TaskList>();
            list << new Objects::TaskList(*reinterpret_cast<Objects::TaskList*>(taskList.data()));
        } else {
            KGAPI2::TaskPtr task = object.dynamicCast<KGAPI2::Task>();
            list << new Objects::Task(*reinterpret_cast<Objects::Task*>(task.data()));
        }
    }

    return list;
}

KGAPI::Object* Tasks::JSONToObject(const QByteArray& jsonData)
{
    QJson::Parser parser;
    const QVariantMap data = parser.parse(jsonData).toMap();

    if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#taskList")) {
        KGAPI2::TaskListPtr taskList = KGAPI2::TasksService::JSONToTaskList(jsonData);
        return new Objects::TaskList(*reinterpret_cast<Objects::TaskList*>(taskList.data()));
    } else if (data.value(QLatin1String("kind")).toString() == QLatin1String("tasks#task")) {
        KGAPI2::TaskPtr task = KGAPI2::TasksService::JSONToTask(jsonData);
        return new Objects::Task(*reinterpret_cast<Objects::Task*>(task.data()));
    }

    return 0;
}

QByteArray Tasks::objectToJSON(KGAPI::Object* object)
{
    if (dynamic_cast< const Objects::TaskList* >(object)) {
        KGAPI2::TaskListPtr taskList(new KGAPI2::TaskList(*reinterpret_cast<KGAPI2::TaskList*>(object)));
        return KGAPI2::TasksService::taskListToJSON(taskList);
    } else if (dynamic_cast< const Objects::Task* >(object)) {
        KGAPI2::TaskPtr task(new KGAPI2::Task(*reinterpret_cast<KGAPI2::Task*>(object)));
        return KGAPI2::TasksService::taskToJSON(task);
    }

    return QByteArray();
}

QUrl Tasks::scopeUrl() const
{
    return Tasks::ScopeUrl;
}

QUrl Tasks::fetchAllTasksUrl(const QString& tasklistID)
{
    return KGAPI2::TasksService::fetchAllTasksUrl(tasklistID);
}

QUrl Tasks::fetchTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return KGAPI2::TasksService::fetchTaskUrl(tasklistID, taskID);
}

QUrl Tasks::createTaskUrl(const QString& tasklistID)
{
    return KGAPI2::TasksService::createTaskUrl(tasklistID);
}

QUrl Tasks::updateTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return KGAPI2::TasksService::updateTaskUrl(tasklistID, taskID);
}

QUrl Tasks::removeTaskUrl(const QString& tasklistID, const QString& taskID)
{
    return KGAPI2::TasksService::removeTaskUrl(tasklistID, taskID);
}

QUrl Tasks::moveTaskUrl(const QString& tasklistID, const QString& taskID, const QString& newParent)
{
    return KGAPI2::TasksService::moveTaskUrl(tasklistID, taskID, newParent);
}

QUrl Tasks::fetchTaskListsUrl()
{
    return KGAPI2::TasksService::fetchTaskListsUrl();
}

QUrl Tasks::createTaskListUrl()
{
    return KGAPI2::TasksService::createTaskListUrl();
}

QUrl Tasks::updateTaskListUrl(const QString& tasklistID)
{
    return KGAPI2::TasksService::updateTaskListUrl(tasklistID);
}

QUrl Tasks::removeTaskListUrl(const QString& tasklistID)
{
    return KGAPI2::TasksService::removeTaskListUrl(tasklistID);
}

QString Tasks::protocolVersion() const
{
    return QString();
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
