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

#ifndef LIBKGAPI_SERVICES_TASKS_H
#define LIBKGAPI_SERVICES_TASKS_H

#include <libkgapi/service.h>
#include <libkgapi/libkgapi_export.h>

namespace KGAPI
{

class Object;

namespace Services
{

class TasksPrivate;

class LIBKGAPI_EXPORT_DEPRECATED Tasks: public KGAPI::Service
{
  public:

    static QUrl ScopeUrl;

    /**
     * Implementation of KGAPI::Service::name().
     */
    static QString serviceName();

    /**
     * Implementation of KGAPI::Service::JSONToObject()
     */
    KGAPI::Object* JSONToObject(const QByteArray& jsonData);

    /**
     * Implementation of KGAPI::Service::objectTOJSON()
     */
    QByteArray objectToJSON(KGAPI::Object* object);

    /**
     * Implementation of KGAPI::Service::parseJSONFeed()
     */
    QList< KGAPI::Object* > parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * Does nothing since Google Tasks API does not support XML.
     */
    QByteArray objectToXML(KGAPI::Object* object);

    /**
     * Does nothing since Google Tasks API does not support XML.
     */
    QList< KGAPI::Object* > parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData);

    /**
     * Does nothing since Google Tasks API does not support XML.
     */
    KGAPI::Object* XMLToObject(const QByteArray& xmlData);

    /**
     * Implementation of KGAPI::Service::protocolVersion().
     *
     * Tasks API does not have any version number, thus this function
     * returns an empty string.
     */
    QString protocolVersion() const;

    /**
     * Returns scope URL for Google Tasks service.
     *
     * https://www.googleapis.com/auth/tasks
     */
    QUrl scopeUrl() const;

    /**
     * Returns URL for KGAPI::Request::Create requests for tasks.
     *
     * @param tasklistID ID of parent task list
     */
    static QUrl createTaskUrl(const QString &tasklistID);

    /**
     * Returns URL for KGAPI::Request::FetchAll requests for tasks.
     *
     * @param tasklistID ID of parent task list
     */
    static QUrl fetchAllTasksUrl(const QString &tasklistID);

    /**
     * Returns URL for KGAPI::Request::Fetch requests for tasks.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to fetch
     */
    static QUrl fetchTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * Returns URL for KGAPI::Request::Update requests for tasks.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to update
     */
    static QUrl updateTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * Returns URL for KGAPI::Request::Remove requests for tasks.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to remove
     */
    static QUrl removeTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * Returns URL for KGAPI::Request::Update request for tasks.
     * This URL is used specially for moving tasks to another parents.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to move
     * @param newParent UID of new parent item
     */
    static QUrl moveTaskUrl(const QString &tasklistID, const QString &taskID, const QString &newParent);

    /**
     * Returns URL for KGAPI::Request::FetchAll requests for tasks lists
     */
    static QUrl fetchTaskListsUrl();

    /**
     * Returns URL for KGAPI::Request::Creata requests for tasks lists.
     */
    static QUrl createTaskListUrl();

    /**
     * Returns URL for KGAPI::Request::Update requests for task lists.
     *
     * @param tasklistID ID of task list to update
     */
    static QUrl updateTaskListUrl(const QString &tasklistID);

    /**
     * Returns URL for KGAPI::Request::Remove requests for task lists.
     *
     * @param tasklistID ID of task list to remove
     */
    static QUrl removeTaskListUrl(const QString &tasklistID);

    /**
     * Returns whether service supports reading data in JSON format.
     *
     * @param urlParam Returns empty string, because Tasks API supports
     * only JSON, so there's no need for explicit parameter for it.
     */
    static bool supportsJSONRead(QString* urlParam);

    /**
     * Returns whether service supports writing data in JSON format.
     *
     * @param urlParam Returns empty string, because Tasks API supports
     * only JSON, so there's no need for explicit parameter for it.
     */
    static bool supportsJSONWrite(QString* urlParam);

  private:
    //krazy:exclude=dpointer
};

} /* namespace Services */

} /* namespace KGAPI */

#endif // LIBKGAPI_SERVICE_TASKS_H
