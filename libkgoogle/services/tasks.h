/*
    libKGoogle - Tasks service
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


#ifndef SERVICE_TASKS_H
#define SERVICE_TASKS_H

#include <libkgoogle/kgoogleservice.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {
  
  class KGoogleObject;
  
  namespace Service {

    class LIBKGOOGLE_EXPORT Tasks: public KGoogleService
    {
      public:
	~Tasks();
	
	/**
	 * Implementation of KGoogleService::JSONToObject()
	 */
	KGoogleObject* JSONToObject(const QByteArray& jsonData);
	
	/**
	 * Implementation of KGoogleService::objectTOJSON()
	 */
	QByteArray objectToJSON(KGoogleObject* object);
	
	/**
	 * Implementation of KGoogleService::parseJSONFeed()
	 */
	QList< KGoogleObject* > parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData = 0);
	
	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */
	QByteArray objectToXML(KGoogleObject* object);
	
	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */	
	QList< KGoogleObject* > parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData = 0);
	
	/**
	 * Does nothing since Google Tasks API does not support XML.
	 */	
	KGoogleObject* XMLToObject(const QByteArray& xmlData);
      
	/**
	 * Implementation of KGoogleService::protocolVersion().
	 * 
	 * Tasks API does not have any version number, thus this function
	 * returns an empty string.
	 */
	const QString protocolVersion();

	/**
	 * Returns scope URL for Google Tasks service.
	 */
	static QString scopeUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Create requests for tasks.
	 */
	static QString createTaskUrl();
	
	/**
	 * Returns URL for KGoogleRequest::FetchAll requests for tasks.
	 */
	static QString fetchAllTasksUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Fetch requests for tasks.
	 */
	static QString fetchTaskUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Update requests for tasks.
	 */
	static QString updateTaskUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Remove requests for tasks.
	 */
	static QString removeTaskUrl();
	
	/**
	 * Returns URL for KGoogleRequest::Update request for tasks.
	 * This URL is used specially for moving tasks to another parents.
	 */
	static QString moveTaskUrl();
	
	/**
	 * Returns URL for KGoogleRequest::FetchAll requests for tasks lists
	 */
	static QString fetchTaskListsUrl();
	
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
	QList< KGoogleObject* > parseTaskListJSONFeed(QVariantList items);
	QList< KGoogleObject* > parseTasksJSONFeed(QVariantList items);
	
	KGoogleObject* JSONToTaskList(QVariantMap jsonData);
	QVariantMap taskListToJSON(KGoogleObject *taskList);
	
	KGoogleObject* JSONToTask(QVariantMap jsonData);
	QVariantMap taskToJSON(KGoogleObject *task);
    };
  } /* namespace Service */
} /* namespace KGoogle */

#endif // SERVICE_TASKS_H
