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


#ifndef LIBKGOOGLE_SERVICES_TASKS_H
#define LIBKGOOGLE_SERVICES_TASKS_H

#include <libkgoogle/service.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {

  class Object;

  namespace Services {

    class LIBKGOOGLE_EXPORT Tasks: public KGoogle::Service
    {
      public:

        static QUrl ScopeUrl;

	/**
	 * Implementation of KGoogle::Service::JSONToObject()
	 */
	KGoogle::Object* JSONToObject(const QByteArray& jsonData);

	/**
	 * Implementation of KGoogle::Service::objectTOJSON()
	 */
	QByteArray objectToJSON(KGoogle::Object* object);

	/**
	 * Implementation of KGoogle::Service::parseJSONFeed()
	 */
	QList< KGoogle::Object* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);

	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */
	QByteArray objectToXML(KGoogle::Object* object);

	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */
	QList< KGoogle::Object* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);

	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */
	KGoogle::Object* XMLToObject(const QByteArray& xmlData);

	/**
	 * Implementation of KGoogle::Service::protocolVersion().
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
	const QUrl& scopeUrl() const;

	/**
	 * Returns URL for KGoogle::Request::Create requests for tasks.
         *
         * @param tasklistID ID of parent task list
	 */
	static QUrl createTaskUrl(const QString &tasklistID);

        /**
	 * Returns URL for KGoogle::Request::FetchAll requests for tasks.
         *
         * @param tasklistID ID of parent task list
	 */
	static QUrl fetchAllTasksUrl(const QString &tasklistID);

	/**
	 * Returns URL for KGoogle::Request::Fetch requests for tasks.
         *
         * @param tasklistID ID of parent task list
         * @param taskID ID of task to fetch
	 */
	static QUrl fetchTaskUrl(const QString &tasklistID, const QString &taskID);

	/**
	 * Returns URL for KGoogle::Request::Update requests for tasks.
         *
         * @param tasklistID ID of parent task list
         * @param taskID ID of task to update
	 */
	static QUrl updateTaskUrl(const QString &tasklistID, const QString &taskID);

	/**
	 * Returns URL for KGoogle::Request::Remove requests for tasks.
         *
         * @param tasklistID ID of parent task list
         * @param taskID ID of task to remove
	 */
	static QUrl removeTaskUrl(const QString &tasklistID, const QString &taskID);

	/**
	 * Returns URL for KGoogle::Request::Update request for tasks.
	 * This URL is used specially for moving tasks to another parents.
         *
         * @param tasklistID ID of parent task list
         * @param taskID ID of task to move
         * @param newParent UID of new parent item
         */
	static QUrl moveTaskUrl(const QString &tasklistID, const QString &taskID, const QString &newParent);

	/**
	 * Returns URL for KGoogle::Request::FetchAll requests for tasks lists
	 */
	static QUrl fetchTaskListsUrl();

        /**
         * Returns URL for KGoogle::Request::Creata requests for tasks lists.
         */
	static QUrl createTaskListUrl();

        /**
         * Returns URL for KGoogle::Request::Update requests for task lists.
         *
         * @param tasklistID ID of task list to update
         */
	static QUrl updateTaskListUrl(const QString &tasklistID);

        /**
         * Returns URL for KGoogle::Request::Remove requests for task lists.
         *
         * @param tasklistID ID of task list to remove
         */
	static QUrl removeTaskListUrl(const QString &tasklistID);

	/**
	 * Returns wheter service supports reading data in JSON format.
	 *
	 * @param urlParam Returns empty string, because Tasks API supports
	 * only JSON, so there's no need for explicit parameter for it.
	 */
	static bool supportsJSONRead(QString* urlParam);

	/**
	 * Returns wheter service supports writing data in JSON format.
	 *
	 * @param urlParam Returns empty string, because Tasks API supports
	 * only JSON, so there's no need for explicit parameter for it.
	 */
	static bool supportsJSONWrite(QString* urlParam);

      private:
	QList< KGoogle::Object* > parseTaskListJSONFeed(const QVariantList &items) const;
	QList< KGoogle::Object* > parseTasksJSONFeed(const QVariantList &items) const;

	KGoogle::Object* JSONToTaskList(QVariantMap jsonData) const;
	QVariantMap taskListToJSON(KGoogle::Object *taskList) const;

	KGoogle::Object* JSONToTask(QVariantMap jsonData) const;
	QVariantMap taskToJSON(KGoogle::Object *task) const;
    };

  } /* namespace Services */

} /* namespace KGoogle */

#endif // SERVICE_TASKS_H
