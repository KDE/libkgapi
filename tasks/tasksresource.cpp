/*
    Akonadi Google - Tasks Resource
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

#include "tasksresource.h"
#include "settings.h"
#include "settingsdialog.h"

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/objects/task.h"
#include "libkgoogle/objects/tasklist.h"
#include "libkgoogle/services/tasks.h"

#include <qstringlist.h>

#include <klocalizedstring.h>
#include <kdialog.h>
#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>

#ifdef WITH_KCAL
#include <kcal/todo.h>
#else
#include <kcalcore/todo.h>
#endif

using namespace Akonadi;
using namespace KGoogle;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

TasksResource::TasksResource(const QString &id): 
  ResourceBase(id)
{
  qRegisterMetaType<KGoogle::Service::Tasks>("Tasks");
  
  m_auth = new KGoogleAuth(Settings::self()->clientId(),
			   Settings::self()->clientSecret(),
			   Service::Tasks::scopeUrl());
  m_gam = new KGoogleAccessManager(m_auth);
  
  connect(m_gam, SIGNAL(replyReceived(KGoogleReply*)),
	  this, SLOT(replyReceived(KGoogleReply*)));
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  
  setObjectName(QLatin1String("GoogleTasksResource"));
  Settings::self()->setResourceId(identifier());
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  
  synchronize();  
}

TasksResource::~TasksResource()
{ 
  
}

void TasksResource::aboutToQuit()
{
  slotAbortRequested();
}

void TasksResource::abort()
{
  cancelTask();
}

void TasksResource::slotAbortRequested()
{
  abort();
}

void TasksResource::configure(WId windowId)
{
  SettingsDialog *settingsDialog = new SettingsDialog(windowId, m_auth);
  if (settingsDialog->exec() == KDialog::Accepted) {
    emit configurationDialogAccepted();
    synchronize();  
  } else {
    emit configurationDialogRejected();
  }
  
  delete settingsDialog;
}

void TasksResource::retrieveItems(const Akonadi::Collection& collection)
{
  emit status(Running, i18n("Preparing to synchronize tasks"));
  ItemFetchJob *fetchJob = new ItemFetchJob(collection);
  fetchJob->fetchScope().fetchFullPayload(false);
  connect (fetchJob, SIGNAL(result(KJob*)),
	   this, SLOT(initialItemFetchJobFinished(KJob*)));
  fetchJob->start();
}

bool TasksResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  Q_UNUSED (parts);
  
  QString url = Service::Tasks::fetchTaskUrl().arg(Settings::self()->taskListId())
					      .arg(item.remoteId());
  KGoogleRequest *request;
  
  request = new KGoogleRequest(QUrl(url),
			       KGoogle::KGoogleRequest::Fetch,
			       "Tasks");
  request->setProperty("Item", QVariant::fromValue(item));
  m_gam->sendRequest(request);
        
  emit status(Running, "Fetching task");
 
  return true;
}

void TasksResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);
  
  if (fetchJob->error()) {
    cancelTask("Failed to fetch initial data.");
    return;
  }

  QString url = Service::Tasks::fetchAllTasksUrl().arg(Settings::self()->taskListId());
						  
  KGoogleRequest *request = new KGoogleRequest(QUrl(url),
					       KGoogleRequest::FetchAll,
					       "Tasks");
  m_gam->sendRequest(request);
  	      
  emit status(Running, i18n("Retrieving list of tasks"));
}

void TasksResource::retrieveCollections()
{
  Akonadi::EntityDisplayAttribute *attr = new Akonadi::EntityDisplayAttribute();
  attr->setDisplayName(Settings::self()->taskListName());
  
  Collection taskList;
  taskList.setRemoteId(Settings::self()->taskListId());
  taskList.setName(Settings::self()->taskListName());
  taskList.setParentCollection(Akonadi::Collection::root());
  taskList.setContentMimeTypes(QStringList() << "text/calendar"
					     << "application/x-vnd.akonadi.calendar.todo");
  taskList.setRights(Collection::ReadOnly |
		     Collection::CanChangeItem |
		     Collection::CanCreateItem |
		     Collection::CanDeleteItem);
  taskList.addAttribute(attr);

  collectionsRetrieved(Collection::List() << taskList);

  /* Use user-friendly name in resource configuration dialog */  
  setAgentName(i18n("Google Tasks - %1", Settings::self()->taskListName()));
}

void TasksResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
 if (!item.hasPayload<TodoPtr>())
    return;

  status(Running, "Creating task...");
  
  TodoPtr todo = item.payload<TodoPtr>();
  Object::Task task(*todo);
 
  Service::Tasks service;
  QString url = Service::Tasks::createTaskUrl().arg(Settings::self()->taskListId());
  QByteArray data = service.objectToJSON(static_cast<KGoogleObject*>(&task));

  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Create,
			       "Tasks");
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (collection);  
}

void TasksResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
 if (!item.hasPayload<TodoPtr>())
    return;
  
  status(Running, "Updating task...");
  
  TodoPtr todo = item.payload<TodoPtr>();
  Object::Task task(*todo);
 
  QString url = Service::Tasks::updateTaskUrl().arg(Settings::self()->taskListId())
					       .arg(item.remoteId());
  Service::Tasks service;
  QByteArray data = service.objectToJSON(dynamic_cast<KGoogleObject*>(&task));
  
  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Update,
			       "Tasks");
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (partIdentifiers);  
}

void TasksResource::itemRemoved(const Akonadi::Item& item)
{
  emit status(Running, i18n("Removing task"));
  
  QString url = Service::Tasks::removeTaskUrl().arg(Settings::self()->taskListId())
					       .arg(item.remoteId());
 
  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Remove,
			       "Tasks");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);
}

void TasksResource::replyReceived(KGoogleReply* reply)
{
  switch (reply->requestType()) {
    case KGoogleRequest::FetchAll:
      taskListReceived(reply);
      break;
      
    case KGoogleRequest::Fetch:
      taskReceived(reply);
      break;
      
    case KGoogleRequest::Create:
      taskCreated(reply);
      break;
      
    case KGoogleRequest::Update:
      taskUpdated(reply);
      break;
      
    case KGoogleRequest::Remove:
      taskRemoved(reply);
      break;
  }
}

void TasksResource::taskListReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to retrieve tasks");
    return;
  }

  Item::List tasks;
  Item::List removed;
  Item::List changed;
  
  QList<KGoogleObject *> allData = reply->replyData();
  foreach (KGoogleObject* replyData, allData) {
    Item item;
    Object::Task *task = static_cast<Object::Task*>(replyData);

    item.setRemoteId(task->id());
    item.setPayload<TodoPtr>(TodoPtr(task));
    item.setMimeType("application/x-vnd.akonadi.calendar.todo");
    
    if (task->deleted()) {
      removed << item;
    } else {
      changed << item;
    }
  }
  
  if (Settings::self()->lastSync().isEmpty())
    itemsRetrieved(changed);
  else
    itemsRetrievedIncremental(changed, removed);

  /* Store the time of this sync. Next time we will only ask for items
   * that changed or were removed since sync */
  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%S"));
  
  emit percent(100);
  emit status(Idle, "Collections synchronized");
}

void TasksResource::taskReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to fetch task");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a task");
    return;
  }     
  Object::Task *task = static_cast<Object::Task*>(data.first());
  
  Item item;
  item.setMimeType("application/x-vnd.akonadi.calendar.todo");
  item.setRemoteId(task->id());
  item.setRemoteRevision(task->etag());
  item.setPayload<TodoPtr>(TodoPtr(task));

  if (task->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);  
  else
    itemRetrieved(item);
  
  emit status(Idle, "Task fetched");
}


void TasksResource::taskCreated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::Created) {
    cancelTask("Failed to create an task");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a task");
    return;
  }     
  Object::Task *task = static_cast<Object::Task*>(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(task->id());
  item.setRemoteRevision(task->etag());

  changeCommitted(item);
  
  status(Idle, "Task created");
}

void TasksResource::taskUpdated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to update task");
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a task");
    return;
  }     
  
  Object::Task *task = static_cast<Object::Task*>(data.first());  
  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(task->id());
  item.setRemoteRevision(task->etag());

  changeCommitted(item);
  
  status(Idle, "Task updated");
}

void TasksResource::taskRemoved(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to remove task");
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();  
  changeCommitted(item);

  status(Idle, "Task removed");
}

AKONADI_RESOURCE_MAIN (TasksResource)