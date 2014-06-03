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

#ifndef LIBKGAPI2_TASKSSERVICE_H
#define LIBKGAPI2_TASKSSERVICE_H

#include <libkgapi2/libkgapi2_export.h>
#include <libkgapi2/types.h>

namespace KGAPI2
{

/**
 * @brief Additional methods for implementing support for Google Tasks service
 *
 * You should never need to use these methods, unless implementing your own Job
 */
namespace TasksService
{

    /**
     * @brief Parses JSON data into a Task object
     *
     * @param jsonData
     */
    LIBKGAPI2_EXPORT TaskPtr JSONToTask(const QByteArray& jsonData);

    /**
     * @brief Serializes a Task object into JSON
     *
     * @param task
     */
    LIBKGAPI2_EXPORT QByteArray taskToJSON(const TaskPtr& task);

    /**
     * @brief Parses JSON data into a TaskList object
     *
     * @param jsonData
     */
    LIBKGAPI2_EXPORT TaskListPtr JSONToTaskList(const QByteArray& jsonData);

    /**
     * @brief Serializes a TaskList into JSON data
     *
     * @param taskList
     */
    LIBKGAPI2_EXPORT QByteArray taskListToJSON(const TaskListPtr& taskList);

    /**
     * @brief Parses JSON feed into list of Tasks or TaskLists
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional information
     *                 about the feed
     */
    LIBKGAPI2_EXPORT ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Returns URL to create a single task
     *
     * @param tasklistID ID of parent task list
     */
    LIBKGAPI2_EXPORT QUrl createTaskUrl(const QString &tasklistID);

    /**
     * @brief Returns URL to fetch all tasks from a single tasklist
     *
     * @param tasklistID ID of parent task list
     */
    LIBKGAPI2_EXPORT QUrl fetchAllTasksUrl(const QString &tasklistID);

    /**
     * @brief Returns URL for fetching a single task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to fetch
     */
    LIBKGAPI2_EXPORT QUrl fetchTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL for updating a task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to update
     */
    LIBKGAPI2_EXPORT QUrl updateTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL for removing a task
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to remove
     */
    LIBKGAPI2_EXPORT QUrl removeTaskUrl(const QString &tasklistID, const QString &taskID);

    /**
     * @brief Returns URL to reparent task.
     *
     * @param tasklistID ID of parent task list
     * @param taskID ID of task to move
     * @param newParent UID of new parent item
     */
    LIBKGAPI2_EXPORT QUrl moveTaskUrl(const QString &tasklistID, const QString &taskID, const QString &newParent);

    /**
     * @brief Returns URL for fetching all tasklists
     */
    LIBKGAPI2_EXPORT QUrl fetchTaskListsUrl();

    /**
     * @brief Returns URL for creating a new tasklist
     */
    LIBKGAPI2_EXPORT QUrl createTaskListUrl();

    /**
     * @brief Returns URL for modifying a tasklist
     *
     * @param tasklistID ID of task list to update
     */
    LIBKGAPI2_EXPORT QUrl updateTaskListUrl(const QString &tasklistID);

    /**
     * @brief Returns URL for deleteing a tasklist
     *
     * @param tasklistID ID of task list to remove
     */
    LIBKGAPI2_EXPORT QUrl removeTaskListUrl(const QString &tasklistID);

} /* namespace TasksServices */

} /* namespace KGAPI2 */

#endif // LIBKGAPI2_TASKSSERVICE_H
