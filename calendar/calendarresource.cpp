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

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/objects/event.h"
#include "libkgoogle/services/calendar.h"

#include <qdebug.h>
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
#include <kcal/event.h>
#include <kcal/calendar.h>
#else
#include <kcalcore/event.h>
#include <kcalcore/calendar.h>
#endif

using namespace Akonadi;
using namespace KGoogle;

#ifdef WITH_KCAL
using namespace KCal;
#else
using namespace KCalCore;
#endif

CalendarResource::CalendarResource(const QString &id): 
  ResourceBase(id)
{
  qRegisterMetaType<KGoogle::Service::Calendar>("Calendar");
  
  m_auth = new KGoogleAuth(Settings::self()->clientId(),
			   Settings::self()->clientSecret(),
			   Service::Calendar::scopeUrl());
  m_gam = new KGoogleAccessManager(m_auth);
  
  connect(m_gam, SIGNAL(replyReceived(KGoogleReply*)),
	  this, SLOT(replyReceived(KGoogleReply*)));
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  

  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);

  synchronize();  
}

CalendarResource::~CalendarResource()
{ 
  
}

void CalendarResource::aboutToQuit()
{
  slotAbortRequested();
}

void CalendarResource::abort()
{
  cancelTask();
}

void CalendarResource::slotAbortRequested()
{
  abort();
}

void CalendarResource::configure(WId windowId)
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

void CalendarResource::retrieveItems(const Akonadi::Collection& collection)
{
  emit status(Running, i18n("Preparing to synchronize calendar"));
  ItemFetchJob *fetchJob = new ItemFetchJob(collection);
  fetchJob->fetchScope().fetchFullPayload(false);
  connect (fetchJob, SIGNAL(result(KJob*)),
	   this, SLOT(initialItemFetchJobFinished(KJob*)));
  fetchJob->start();
}

bool CalendarResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  Q_UNUSED (parts);
  
  QString url = Service::Calendar::fetchUrl().arg(Settings::self()->calendarId())
					     .arg("private")
					     .arg(item.remoteId());
  KGoogleRequest *request;
  
  request = new KGoogleRequest(QUrl(url),
			       KGoogle::KGoogleRequest::Fetch,
			       "Calendar");
  request->setProperty("Item", QVariant::fromValue(item));
  m_gam->sendRequest(request);
        
  emit status(Running, "Fetching event");
 
  return true;
}

void CalendarResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);
  
  if (fetchJob->error()) {
    cancelTask("Failed to fetch initial data.");
    return;
  }

  QString url = Service::Calendar::fetchAllUrl().arg(Settings::self()->calendarId())
						.arg("private");
  KGoogleRequest *request = new KGoogleRequest(QUrl(url),
					       KGoogleRequest::FetchAll,
					       "Calendar");
  m_gam->sendRequest(request);
  	      
  emit status(Running, i18n("Retrieving list of events"));
}

void CalendarResource::retrieveCollections()
{
  Akonadi::EntityDisplayAttribute *attr = new Akonadi::EntityDisplayAttribute();
  attr->setDisplayName(Settings::self()->calendarName());
  
  Collection calendar;
  calendar.setRemoteId(Settings::self()->calendarId());
  calendar.setName(Settings::self()->calendarName());
  calendar.setParentCollection(Akonadi::Collection::root());
  calendar.setContentMimeTypes(QStringList() << "text/calendar" );
  calendar.setRights(Collection::ReadOnly |
		     Collection::CanChangeItem |
		     Collection::CanCreateItem |
		     Collection::CanDeleteItem);
  calendar.addAttribute(attr);

  collectionsRetrieved(Collection::List() << calendar);
}

void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
 if (!item.hasPayload<EventPtr>())
    return;

  status(Running, "Creating event...");
  
  EventPtr event = item.payload<EventPtr>();
  Object::Event kevent(*event);
 
  Service::Calendar service;
  QString url = Service::Calendar::createUrl().arg(Settings::self()->calendarId())
					      .arg("private");
  QByteArray data = service.objectToJSON(static_cast<KGoogleObject*>(&kevent));

  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Create,
			       "Calendar");
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (collection);  
}

void CalendarResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
 if (!item.hasPayload<EventPtr>())
    return;
  
  status(Running, "Updating event...");
  
  EventPtr event = item.payload<EventPtr>();
  Object::Event kevent(*event);
 
  QString url = Service::Calendar::updateUrl().arg(Settings::self()->calendarId())
					      .arg("private")
					      .arg(item.remoteId());
  Service::Calendar service;
  QByteArray data = service.objectToJSON(dynamic_cast<KGoogleObject*>(&kevent));
  
  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Update,
			       "Calendar");
  request->setRequestData(data, "application/json");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (partIdentifiers);  
}

void CalendarResource::itemRemoved(const Akonadi::Item& item)
{
  emit status(Running, i18n("Removing event"));
  
  QString url = Service::Calendar::removeUrl().arg(Settings::self()->calendarId())
					      .arg("private")
					      .arg(item.remoteId());
 
  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Remove,
			       "Calendar");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);
}

void CalendarResource::replyReceived(KGoogleReply* reply)
{
  qDebug() << "reply received, " << reply->replyData().count() << " items fetched";
  
  switch (reply->requestType()) {
    case KGoogleRequest::FetchAll:
      eventListReceived(reply);
      break;
      
    case KGoogleRequest::Fetch:
      eventReceived(reply);
      break;
      
    case KGoogleRequest::Create:
      eventCreated(reply);
      break;
      
    case KGoogleRequest::Update:
      eventUpdated(reply);
      break;
      
    case KGoogleRequest::Remove:
      eventRemoved(reply);
      break;
  }
}

void CalendarResource::eventListReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to retrieve events");
    return;
  }

  Item::List events;
  Item::List removed;
  Item::List changed;
  
  QList<KGoogleObject *> allData = reply->replyData();
  foreach (KGoogleObject* replyData, allData) {
    Item item;
    Object::Event *event = static_cast<Object::Event*>(replyData);

    item.setRemoteId(event->id());
    item.setPayload<EventPtr>(EventPtr(event));
    item.setMimeType("application/x-vnd.akonadi.calendar.event");
    
    if (event->deleted()) {
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

void CalendarResource::eventReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to fetch event");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }     
  Object::Event *event = static_cast<Object::Event*>(data.first());
  
  Item item;
  item.setMimeType("application/x-vnd.akonadi.calendar.event");
  item.setRemoteId(event->id());
  item.setRemoteRevision(event->etag());
  item.setPayload<EventPtr>(EventPtr(event));

  if (event->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);  
  else
    itemRetrieved(item);
  
  emit status(Idle, "Event fetched");
}


void CalendarResource::eventCreated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::Created) {
    cancelTask("Failed to create an event");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }     
  Object::Event *event = static_cast<Object::Event*>(data.first());
  
  Item newItem;
  newItem.setRemoteId(event->id());
  newItem.setRemoteRevision(event->etag());
  newItem.setMimeType("application/x-vnd.akonadi.calendar.event");
  newItem.setPayload<EventPtr>(EventPtr(event));

  changeCommitted(newItem);
  
  status(Idle, "Event created");
}

void CalendarResource::eventUpdated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to update event");
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }     
  Object::Event *event = static_cast<Object::Event*>(data.first());
  
  Item oldItem = reply->request()->property("Item").value<Item>();
  Item newItem;

  newItem.setRemoteId(event->id());
  newItem.setRemoteRevision(event->etag());
  newItem.setMimeType("application/x-vnd.akonadi.calendar.event");
  newItem.setPayload<EventPtr>(EventPtr(event));
  
  changeCommitted(newItem);
  
  status(Idle, "Event changed");
}

void CalendarResource::eventRemoved(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to remove event");
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();
  
  changeCommitted(item);

  status(Idle, "Event removed");
}

AKONADI_RESOURCE_MAIN (CalendarResource)