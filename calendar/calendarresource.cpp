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

#include <qstringlist.h>

#include <klocalizedstring.h>
#include <kdialog.h>
#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
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

  setNeedsNetwork(true);
  setOnline(false);

  m_auth = new KGoogleAuth(Settings::self()->clientId(),
			   Settings::self()->clientSecret(),
			   Service::Calendar::scopeUrl());
  connect(m_auth, SIGNAL(tokensRecevied(QString,QString)),
	  this, SLOT(tokensReceived()));

  m_gam = new KGoogleAccessManager(m_auth);
  connect(m_gam, SIGNAL(error(QString,int)),
	  this, SLOT(slotGAMError(QString,int)));
  connect(m_gam, SIGNAL(replyReceived(KGoogleReply*)),
	  this, SLOT(replyReceived(KGoogleReply*)));
  connect(m_gam, SIGNAL(requestFinished(KGoogleRequest*)),
	  this, SLOT(commitItemsList()));
  connect(m_gam, SIGNAL(authError(QString)),
          this, SLOT(authError(QString)));

  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));

  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
}

CalendarResource::~CalendarResource()
{
  delete m_gam;
  delete m_auth;
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

void CalendarResource::slotGAMError(const QString &msg, const int errorCode)
{
  cancelTask(msg);

  Q_UNUSED(errorCode);
}

void CalendarResource::authError (const QString &error)
{
  cancelTask(error);

  setOnline(false);
  emit status(Broken, error);
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

  emit status(Running, i18n("Fetching event"));

  return true;
}

void CalendarResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);

  if (fetchJob->error()) {
    cancelTask(i18n("Failed to fetch initial data."));
    return;
  }

  QUrl url(Service::Calendar::fetchAllUrl().arg(Settings::self()->calendarId())
					   .arg("private"));
  if (!Settings::self()->lastSync().isEmpty()) {
    url.addQueryItem("updated-min", Settings::self()->lastSync());
  }

  KGoogleRequest *request = new KGoogleRequest(url,
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
  calendar.setContentMimeTypes(QStringList() << "application/x-vnd.akonadi.calendar.event" );
  calendar.setRights(Collection::ReadOnly |
		     Collection::CanChangeItem |
		     Collection::CanCreateItem |
		     Collection::CanDeleteItem);
  calendar.addAttribute(attr);

  if (Settings::self()->refreshCalendar()) {
    Akonadi::CachePolicy policy;
    policy.setInheritFromParent(true);
    policy.setIntervalCheckTime(Settings::self()->refreshInterval());
    policy.setSyncOnDemand(true);
    calendar.setCachePolicy(policy);
  }

  /* Use user-friendly name in resource configuration dialog */
  setAgentName(i18n("Google Calendar - %1", Settings::self()->calendarName()));  

  collectionsRetrieved(Collection::List() << calendar);
}

void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<EventPtr>()) {
    cancelTask(i18n("Invalid Payload"));
    return;
  }

  status(Running, i18n("Creating event..."));

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
  if (!item.hasPayload<EventPtr>()) {
    cancelTask(i18n("Invalid Payload"));
    return;
  }

  status(Running, i18n("Updating event..."));

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
    cancelTask(i18n("Failed to retrieve events"));
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
    item.setRemoteRevision(event->etag());
    item.setPayload<EventPtr>(EventPtr(event));
    item.setMimeType("application/x-vnd.akonadi.calendar.event");

    if (event->deleted()) {
      m_removedItems << item;
    } else {
      m_changedItems << item;
    }
  }

  /* Wait for requestFinished() signal and then commit all items at once in
   * commitItemsList() */
}

void CalendarResource::eventReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to fetch event"));
    return;
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create an event"));
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

  emit status(Idle, i18n("Event fetched"));
  taskDone();
}


void CalendarResource::eventCreated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::Created) {
    cancelTask(i18n("Failed to create an event"));
    return;
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create an event"));
    return;
  }
  Object::Event *event = static_cast<Object::Event*>(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(event->id());
  item.setRemoteRevision(event->etag());
  changeCommitted(item);

  item.setPayload<EventPtr>(EventPtr(event));
  Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(item);
  connect (modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));

  status(Idle, i18n("Event created"));
  taskDone();
}

void CalendarResource::eventUpdated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to update event"));
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to update an event"));
    return;
  }
  Object::Event *event = static_cast<Object::Event*>(data.first());

  Item newItem(reply->request()->property("Item").value<Item>());
  newItem.setRemoteId(event->id());
  newItem.setRemoteRevision(event->etag());
  newItem.setPayload<EventPtr>(EventPtr(event));
  changeCommitted(newItem);

  status(Idle, i18n("Event changed"));
  taskDone();
}

void CalendarResource::eventRemoved(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to remove event"));
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();  
  changeCommitted(item);

  status(Idle, i18n("Event removed"));
  taskDone();
}

void CalendarResource::tokensReceived()
{
  if (m_auth->accessToken().isEmpty()) {
    emit status(Broken, i18n("Failed to fetch tokens"));
  } else {
    setOnline(true);
    synchronize();
  }
}

void CalendarResource::commitItemsList()
{
  if (Settings::self()->lastSync().isEmpty())
    itemsRetrieved(m_changedItems);
  else
    itemsRetrievedIncremental(m_changedItems, m_removedItems);

  /* Store the time of this sync. Next time we will only ask for items
   * that changed or were removed since sync
   * The conversion to UTC is a workaround for Google having troubles to accept +XX:00 timezones
   */
  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));

  emit percent(100);
  emit status(Idle, i18n("Collections synchronized"));
  taskDone();

  m_changedItems.clear();
  m_removedItems.clear();
}


AKONADI_RESOURCE_MAIN (CalendarResource)
