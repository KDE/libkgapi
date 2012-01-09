/*
    Akonadi Google - Calendar Resource
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


#include "calendarresource.h"
#include "settings.h"

#include <libkgoogle/reply.h>
#include <libkgoogle/objects/task.h>
#include <libkgoogle/objects/tasklist.h>
#include <libkgoogle/services/tasks.h>

#include <klocalizedstring.h>
#include <kdebug.h>

#include <akonadi/itemmodifyjob.h>
#include <akonadi/entitydisplayattribute.h>

#ifdef WITH_KCAL
#include <kcal/event.h>
#include <kcal/calendar.h>
#else
#include <kcalcore/event.h>
#include <kcalcore/calendar.h>
#include <services/tasks.h>
#endif

using namespace Akonadi;
using namespace KGoogle;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

bool CalendarResource::taskListReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    emit status(Broken, i18n("Failed to fetch task lists"));
    return false;
  }

  QStringList taskLists = Settings::self()->taskLists();

  QList< Object* > data = reply->replyData();
  foreach (Object* replyData, data) {

    Objects::TaskList *taskList = static_cast< Objects::TaskList* >(replyData);

    if (!taskLists.contains(taskList->uid()))
      continue;

    Collection collection;
    collection.setRemoteId(taskList->uid());
    collection.setParentCollection(m_collections.first());
    collection.setContentMimeTypes(QStringList() << Todo::todoMimeType());
    collection.setName(taskList->title());
    collection.setRights(Collection::AllRights);

    EntityDisplayAttribute *attr = new EntityDisplayAttribute;
    attr->setDisplayName(taskList->title());
    attr->setIconName("text-calendar");
    collection.addAttribute(attr);

    m_collections.append(collection);

  }

  return true;
}

bool CalendarResource::commitTaskLists(KGoogle::Request *request)
{
  Q_UNUSED (request);

  m_fetchedTaskLists = true;

  if (m_fetchedCalendars && m_fetchedTaskLists) {
    collectionsRetrieved(m_collections);

    m_collections.clear();
    m_fetchedCalendars = false;
    m_fetchedTaskLists = false;

    return true;
  }

  return false;
}

bool CalendarResource::taskReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to fetch task: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
  }

  Objects::Task *task = static_cast< Objects::Task* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();  
  item.setRemoteId(task->uid());
  item.setRemoteRevision(task->etag());
  item.setMimeType(Todo::todoMimeType());
  item.setPayload< TodoPtr >(TodoPtr(task));

  if (static_cast< Objects::Task* >(task)->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);
  else
    itemRetrieved(item);

  emit status(Idle, i18n("Todo fetched"));

  return true;
}

bool CalendarResource::tasksReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to fetch tasks: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  foreach (Object* replyData, data) {

    Objects::Task *task = static_cast< Objects::Task* >(replyData);

    Item item;
    item.setRemoteId(task->uid());
    item.setRemoteRevision(task->etag());
    item.setPayload< TodoPtr >(TodoPtr(task));
    item.setMimeType(Todo::todoMimeType());
    item.setParentCollection(reply->request()->property("Collection").value<Collection>());

    if (task->deleted())
      m_removedTodos << item;
    else
      m_changedTodos << item;

  }

  /* Don't emit taskDone() - will be emitted when commitTasks() is called */
  return false;
}

bool CalendarResource::taskCreated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to create a task: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
  }

  Objects::Task *task = static_cast< Objects::Task* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(task->uid());
  item.setRemoteRevision(task->etag());
  item.setMimeType(Todo::todoMimeType());
  item.setPayload< TodoPtr >(TodoPtr(task));
  item.setParentCollection(reply->request()->property("Collection").value<Collection>());

  emit status(Idle, i18n("Todo created"));

  changeCommitted(item);

  Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(item);
  connect (modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));

  return true;
}

bool CalendarResource::taskUpdated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to update task: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
  }

  Objects::Task *task = static_cast< Objects::Task* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteRevision(task->etag());
  item.setPayload< TodoPtr >(TodoPtr(task));
  emit status(Idle, i18n("Todo updated"));

  changeCommitted(item);

  return true;
}

bool CalendarResource::taskRemoved(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to delete task: %1").arg(reply->errorString()));
    return false;
  }

  Item item = reply->request()->property("Item").value<Item>();
  changeCommitted(item);

  emit status(Idle, i18n("Todo deleted"));

  return true;
}

bool CalendarResource::commitTasks(KGoogle::Request *request)
{
  Collection collection = request->property("Collection").value< Collection >();
  QString collectionId = collection.remoteId();
  bool updated = false;

  QStringList tasklists = Settings::self()->tasksSync();
  foreach (const QString &tasklist, tasklists) {
    if (tasklist.startsWith(collectionId + ',')) {
      itemsRetrievedIncremental(m_changedTodos, m_removedTodos);
      tasklists.removeAll(tasklist);
      tasklists.append(collectionId + ',' + KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));
      updated = true;
      break;
    }
  }

  if (updated == false) {
    itemsRetrieved(m_changedTodos);
    tasklists.append(collectionId + ',' + KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));
  }

  Settings::self()->setTasksSync(tasklists);
  Settings::self()->writeConfig();

  m_changedTodos.clear();
  m_removedTodos.clear();

  emit percent(100);
  emit status(Idle, i18n("Collections synchronized"));

  taskDone();

  return true;
}