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
#include "settingsdialog.h"

#include <libkgoogle/common.h>
#include <libkgoogle/account.h>
#include <libkgoogle/accessmanager.h>
#include <libkgoogle/auth.h>
#include <libkgoogle/request.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/objects/calendar.h>
#include <libkgoogle/objects/event.h>
#include <libkgoogle/objects/task.h>
#include <libkgoogle/objects/tasklist.h>
#include <libkgoogle/services/calendar.h>
#include <libkgoogle/services/tasks.h>

#include <qstringlist.h>
#include <qmetatype.h>

#include <klocalizedstring.h>
#include <kdialog.h>
#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
#include <akonadi/collectionfetchscope.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemmodifyjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>
#include <akonadi/cachepolicy.h>

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

CalendarResource::CalendarResource(const QString &id):
  ResourceBase(id),
  m_fetchedCalendars(false),
  m_fetchedTaskLists(false)
{
  qRegisterMetaType< KGoogle::Services::Calendar >("Calendar");
  qRegisterMetaType< KGoogle::Services::Tasks >("Tasks");

  Auth *auth = Auth::instance();
  auth->setKWalletFolder("Akonadi Google");

  setNeedsNetwork(true);
  setOnline(true);

  m_gam = new AccessManager();
  connect(m_gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));
  connect(m_gam, SIGNAL(replyReceived(KGoogle::Reply*)),
	  this, SLOT(replyReceived(KGoogle::Reply*)));
  connect(m_gam, SIGNAL(requestFinished(KGoogle::Request*)),
	  this, SLOT(commitItemsList(KGoogle::Request*)));

  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));

  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  changeRecorder()->itemFetchScope().setAncestorRetrieval(ItemFetchScope::All);
  changeRecorder()->fetchCollection(true);
  changeRecorder()->collectionFetchScope().setAncestorRetrieval(CollectionFetchScope::All);
}

CalendarResource::~CalendarResource()
{
  delete m_gam;
}

void CalendarResource::aboutToQuit()
{
  slotAbortRequested();
}

void CalendarResource::abort()
{
  cancelTask(i18n("Aborted"));
}

void CalendarResource::slotAbortRequested()
{
  abort();
}

void CalendarResource::error(const KGoogle::Error errCode, const QString &msg)
{
  cancelTask(msg);

  if ((errCode == AuthError) || (errCode == BackendNotReady)) {
    status(Broken, msg);
  }
}

void CalendarResource::configure(WId windowId)
{
  SettingsDialog *settingsDialog = new SettingsDialog(windowId);

  if (settingsDialog->exec() == KDialog::Accepted) {
    emit configurationDialogAccepted();
    synchronize();
  } else {
    emit configurationDialogRejected();
  }

  delete settingsDialog;
}

void CalendarResource::retrieveItems(const Akonadi::Collection& collection)
{
  ItemFetchJob *fetchJob = new ItemFetchJob(collection, this);
  connect(fetchJob, SIGNAL(finished(KJob*)),
          this, SLOT(cachedItemsRetrieved(KJob*)));
  connect(fetchJob, SIGNAL(finished(KJob*)),
          fetchJob, SLOT(deleteLater()));

  fetchJob->fetchScope().fetchFullPayload(false);
  fetchJob->setProperty("Collection", qVariantFromValue(collection));
  fetchJob->start();
}

void CalendarResource::cachedItemsRetrieved(KJob* job)
{
  QUrl url;
  QString service;
  QStringList syncs;

  Collection collection = job->property("Collection").value<Collection>();

  /* Can't fetch items for root collection. */
  if (collection == Akonadi::Collection::root())
    return;

  if (collection.contentMimeTypes().contains(Event::eventMimeType())) {

    emit status(Running, i18n("Synchronizing calendar %1").arg(collection.name()));

    service = "Calendar";
    url = Services::Calendar::fetchAllUrl(collection.remoteId(), "private");
    syncs = Settings::self()->calendarSync();

  } else if (collection.contentMimeTypes().contains(Todo::todoMimeType())) {

    emit status(Running, i18n("Synchronizing tasklist %1").arg(collection.name()));

    service = "Tasks";
    url = Services::Tasks::fetchAllTasksUrl(collection.remoteId());
    syncs = Settings::self()->tasksSync();

  } else {

    emit cancelTask(i18n("Invalid collection"));
    return;

  }

  QString lastSync;
  if (!syncs.isEmpty()) {
    foreach (QString sync, syncs) {
      if (sync.startsWith(collection.remoteId() + ',')) {
        lastSync = sync.mid(sync.indexOf(',') + 1);
        break;
      }
    }
  }

  if (!lastSync.isEmpty())
    url.addQueryItem("updated-min", lastSync);

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  Request *request = new Request(url, Request::FetchAll, service, account);
  request->setProperty("Collection", qVariantFromValue(collection));
  m_gam->sendRequest(request);
}


bool CalendarResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  Q_UNUSED (parts);

  QString service;
  QUrl url;

  if (item.parentCollection().contentMimeTypes().contains(Event::eventMimeType())) {

    emit status(Running, i18n("Fetching an event"));
    service = "Calendar";
    url = Services::Calendar::fetchUrl(Settings::self()->account(), "private", item.remoteId());

  } else if (item.parentCollection().contentMimeTypes().contains(Todo::todoMimeType())) {

    emit status(Running, i18n("Fetching a task"));
    service = "Tasks";
    url = Services::Tasks::fetchTaskUrl(item.parentCollection().remoteId(), item.remoteId());

  }

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BackendNotReady &e) {
    emit status(Broken, e.what());
    return false;
  }

  Request *request = new Request(url, KGoogle::Request::Fetch, service, account);
  request->setProperty("Item", QVariant::fromValue(item));
  m_gam->sendRequest(request);

  return true;
}

void CalendarResource::retrieveCollections()
{
  Request *request;

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  emit status(Running, i18n("Synchronizing collections..."));

  Akonadi::EntityDisplayAttribute *attr = new Akonadi::EntityDisplayAttribute();
  attr->setDisplayName(Settings::self()->account());

  Collection collection;
  collection.setName(identifier());
  collection.setRemoteId(identifier());
  collection.setParentCollection(Akonadi::Collection::root());
  collection.setContentMimeTypes(QStringList() << Collection::mimeType());
  collection.addAttribute(attr);

  m_collections.clear();
  m_collections.append(collection);

  request = new Request(Services::Calendar::fetchAllUrl("default", "allcalendars"),
                        KGoogle::Request::FetchAll, "Calendar", account);
  request->setProperty("isCollection", true);
  m_gam->sendRequest(request);

  request = new Request(Services::Tasks::fetchTaskListsUrl(),
                        KGoogle::Request::FetchAll, "Tasks", account);
  request->setProperty("isCollection", true);
  m_gam->sendRequest(request);
}

void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  QString service;
  QUrl url;
  QByteArray data;

  if (item.mimeType() == Event::eventMimeType()) {

    emit status(Running, i18n("Creating a new event..."));

    EventPtr event = item.payload< EventPtr >();
    Objects::Event kevent(*event);

    service = "Calendar";
    url = Services::Calendar::createUrl(Settings::self()->account(), "private");

    Services::Calendar service;
    data = service.objectToJSON(static_cast< KGoogle::Object* >(&kevent));

  } else if (item.mimeType() == Todo::todoMimeType()) {

    emit status(Running, i18n("Creating a new todo..."));

    TodoPtr todo = item.payload< TodoPtr >();
    Objects::Task ktodo(*todo);

    service = "Tasks";
    url = Services::Tasks::createTaskUrl(collection.remoteId());

    Services::Tasks service;
    data = service.objectToJSON(static_cast< KGoogle::Object* >(&ktodo));

  } else {
    cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
    return;
  }

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status (Broken, e.what());
    return;
  }

  Request *request = new Request(url, Request::Create, service, account);
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));
  request->setProperty("Collection", QVariant::fromValue(collection));

  m_gam->sendRequest(request);
}

void CalendarResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  QString service;
  QUrl url;
  QByteArray data;

  if (item.mimeType() == Event::eventMimeType()) {

    status(Running, i18n("Updating an event..."));

    EventPtr event = item.payload< EventPtr >();
    Objects::Event kevent(*event);

    url = Services::Calendar::updateUrl(Settings::self()->account(), "private", item.remoteId());
    service = "Calendar";

    Services::Calendar service;
    data = service.objectToJSON(static_cast< KGoogle::Object* >(&kevent));

  } else if (item.mimeType() == Todo::todoMimeType()) {

    status(Running, i18n("Updating a todo..."));

    TodoPtr todo = item.payload< TodoPtr >();
    Objects::Task ktodo(*todo);

    url = Services::Tasks::updateTaskUrl(item.parentCollection().remoteId(), item.remoteId());
    service = "Tasks";

    Services::Tasks service;
    data = service.objectToJSON(static_cast< KGoogle::Object* >(&ktodo));

  } else {
    cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
    return;
  }

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  Request *request = new Request(url, Request::Update, service, account);
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);

  Q_UNUSED (partIdentifiers);
}

void CalendarResource::itemRemoved(const Akonadi::Item& item)
{
  QString service;
  QUrl url;

  if (item.mimeType() == Event::eventMimeType()) {

    service = "Calendar";
    url = Services::Calendar::removeUrl(Settings::self()->account(), "private", item.remoteId());

  } else if (item.mimeType() == Todo::todoMimeType()) {

    service = "Tasks";
    url = Services::Tasks::removeTaskUrl(item.parentCollection().remoteId(), item.remoteId());

  } else {
    cancelTask(i18n("Unknown payload type '%1'").arg(item.mimeType()));
    return;
  }

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  } 
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  Request *request = new Request(url, Request::Remove, service, account);
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);
}

void CalendarResource::replyReceived(KGoogle::Reply* reply)
{
  switch (reply->requestType()) {
    case Request::FetchAll:
      itemsReceived(reply);
      break;

    case Request::Fetch:
      itemReceived(reply);
      break;

    case Request::Create:
      itemCreated(reply);
      break;

    case Request::Update:
      itemUpdated(reply);
      break;

    case Request::Remove:
      itemRemoved(reply);
      break;
  }
}

void CalendarResource::itemsReceived(KGoogle::Reply *reply)
{
  if (reply->serviceName() == "Calendar") {

    if (reply->request()->hasProperty("isCollection") &&
        reply->request()->property("isCollection").toBool()) {

      if (calendarsReceived(reply))
        taskDone();

    } else {

      if (eventsReceived(reply))
        taskDone();

    }

  } else if (reply->serviceName() == "Tasks") {

    if (reply->request()->hasProperty("isCollection") &&
        reply->request()->property("isCollection").toBool()) {

      if (taskListReceived(reply))
        taskDone();

    } else {

      if (tasksReceived(reply))
        taskDone();

    }

  } else {

    cancelTask(i18n("Received invalid reply"));

  }

  delete reply;

}

void CalendarResource::itemReceived(Reply* reply)
{
  if (reply->serviceName() == "Calendar") {

    if (eventReceived(reply))
      taskDone();

  } else if (reply->serviceName() == "Tasks") {

    if (taskReceived(reply))
      taskDone();

  } else {

    cancelTask(i18n("Received an invalid reply"));

  }

  delete reply;

}


void CalendarResource::itemCreated(Reply* reply)
{
  if (reply->serviceName() == "Calendar") {

    if (eventCreated(reply))
      taskDone();

  } else if (reply->serviceName() == "Tasks") {

    if (taskCreated(reply))
      taskDone();

  } else {

    cancelTask(i18n("Received an invalid reply"));

  }

  delete reply;

}

void CalendarResource::itemUpdated(Reply* reply)
{
  if (reply->serviceName() == "Calendar") {

    if (eventUpdated(reply))
      taskDone();

  } else if (reply->serviceName() == "Tasks") {

    if (taskUpdated(reply))
      taskDone();

  } else {

    cancelTask(i18n("Received an invalid reply"));

  }

  delete reply;

}

void CalendarResource::itemRemoved(Reply* reply)
{
  if (reply->serviceName() == "Calendar") {

    if (eventRemoved(reply))
      taskDone();

  } else if (reply->serviceName() == "Tasks") {

    if (taskRemoved(reply))
      taskDone();

  } else {

    cancelTask(i18n("Received an invalid reply"));

  }

  delete reply;
}


void CalendarResource::commitItemsList(KGoogle::Request *request)
{

  if (request->serviceName() == "Calendar") {

    if (request->hasProperty("isCollection") &&
        request->property("isCollection").toBool()) {

      if (commitCalendars(request))
        taskDone();

    } else {

      if (commitEvents(request))
        taskDone();

    }

  } else if (request->serviceName() == "Tasks") {

    if (request->hasProperty("isCollection") &&
        request->property("isCollection").toBool()) {

      if (commitTaskLists(request))
        taskDone();

    } else {

      if (commitTasks(request))
        taskDone();

    }

  }

}


AKONADI_RESOURCE_MAIN (CalendarResource);