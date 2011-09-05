/*
    Akonadi Google - Contacts resource
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

#include "contactsresource.h"
#include "settingsdialog.h"
#include "settings.h"
#include "photojob.h"

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/objects/contact.h"
#include "libkgoogle/services/addressbook.h"

#include <qstringlist.h>
#include <kdebug.h>

#include <klocalizedstring.h>
#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>

#include <kabc/addressee.h>
#include <kabc/picture.h>

using namespace Akonadi;
using namespace KGoogle;

ContactsResource::ContactsResource(const QString &id): 
  ResourceBase(id)
{
  /* Register all types we are going to use in this resource */
  qRegisterMetaType<Service::Addressbook>("Addressbook");
  
  
  setObjectName(QLatin1String("GoogleContactsResource"));
  setNeedsNetwork(true);
  
  m_auth = new KGoogleAuth(Settings::self()->clientId(),
			   Settings::self()->clientSecret(),
			   Service::Addressbook::scopeUrl());
  m_gam = new KGoogleAccessManager(m_auth);
  
  connect(m_gam, SIGNAL(replyReceived(KGoogleReply*)),
	  this, SLOT(replyReceived(KGoogleReply*)));
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);

  if (m_auth->accessToken().isEmpty()) {
    setOnline(false);
  } else {
    setOnline(true);
    synchronize();
  }
}

ContactsResource::~ContactsResource()
{
  
}

void ContactsResource::aboutToQuit()
{
  slotAbortRequested();
}

void ContactsResource::abort()
{
  cancelTask();
}

void ContactsResource::slotAbortRequested()
{
  abort();
}

void ContactsResource::configure(WId windowId)
{
  SettingsDialog *dlg = new SettingsDialog(windowId, m_auth);
  dlg->exec();

  emit configurationDialogAccepted();
  if (!m_auth->accessToken().isEmpty()) {
    setOnline(true);
    synchronize();
  } else {
    setOnline(false);
  }

  delete dlg;
}

void ContactsResource::retrieveItems(const Akonadi::Collection& collection)
{
    if (collection.remoteId() == "googleContacts") {
      emit status(Running, i18n("Preparing synchronization of contacts"));
      ItemFetchJob *fetchJob = new ItemFetchJob(collection);
      fetchJob->fetchScope().fetchFullPayload(false);
      connect (fetchJob, SIGNAL(result(KJob*)),
	       this, SLOT(initialItemFetchJobFinished(KJob*)));
      fetchJob->start();
    } else if (collection.remoteId() == "contactPhoto") {
	/* TODO: Implement me! */
    } else {
      Q_ASSERT ("Unknown collection!");
      cancelTask();
    }
}

bool ContactsResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  Q_UNUSED (parts);
  
  if (item.mimeType() == "text/directory") {
    QString url = Service::Addressbook::fetchUrl().arg("default").arg(item.remoteId());

    KGoogleRequest *request;
    request = new KGoogleRequest(QUrl(url),
				 KGoogle::KGoogleRequest::Fetch,
				 "Addressbook");
    
    request->setProperty("Item", QVariant::fromValue(item));
    m_gam->sendRequest(request);
        
    emit status(Running, "Fetching contact");
  }
  
  return true;
}

void ContactsResource::retrieveCollections()
{
  
  EntityDisplayAttribute* displayAttribute = new EntityDisplayAttribute();
  displayAttribute->setDisplayName(i18n("Google Contacts"));

  Collection contacts;
  contacts.setRemoteId("googleContacts");
  contacts.setName(i18n("Google Contacts"));
  contacts.setParentCollection(Akonadi::Collection::root());
  contacts.setContentMimeTypes(QStringList() << KABC::Addressee::mimeType());
  contacts.setRights(Collection::CanDeleteItem |
		     Collection::CanCreateItem |
		     Collection::CanChangeItem);
  contacts.addAttribute(displayAttribute);

  collectionsRetrieved(Collection::List() << contacts);
  
  /* Use user-friendly name in resource configuration dialog */
  setAgentName(i18n("Google Contacts"));    
}

void ContactsResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);
  
  if (fetchJob->error()) {
    cancelTask("Failed to fetch initial data.");
    return;
  }

  QString url = Service::Addressbook::fetchAllUrl().arg("default");
  if (!Settings::self()->lastSync().isEmpty())
    url.append("?updated-min="+Settings::self()->lastSync());

  KGoogleRequest *request = new KGoogleRequest(QUrl(url),
					       KGoogleRequest::FetchAll,
					       "Addressbook");
  m_gam->sendRequest(request);
  	      
  emit status(Running, i18n("Retrieving list of contacts"));
}

void ContactsResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;

  status(Running, "Creating contact...");
  
  Service::Addressbook service;
  Object::Contact contact;
  QString url = Service::Addressbook::createUrl().arg("default");
  QByteArray data;
  KGoogleRequest *request;
  
  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  contact.fromKABC(&addressee);
  data = service.objectToXML(&contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom='http://www.w3.org/2005/Atom' xmlns:gd='http://schemas.google.com/g/2005'>"
	       "<atom:category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/contact/2008#contact'/>");
  data.append("</atom:entry>");
  
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Create,
			       "Addressbook");
  request->setRequestData(data, "application/atom+xml");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (collection);
}

void ContactsResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;
  
  status(Running, "Updating contact...");
  
  QString url = Service::Addressbook::updateUrl().arg("default").arg(item.remoteId());
  QByteArray data;
  Object::Contact contact;
  Service::Addressbook service;
  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  
  contact.fromKABC(&addressee);
  data = service.objectToXML(&contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom='http://www.w3.org/2005/Atom' xmlns:gd='http://schemas.google.com/g/2005'>"
	       "<atom:category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/contact/2008#contact'/>");
  data.append("</atom:entry>");
  

  KGoogleRequest *request;  
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Update,
			       "Addressbook");
  request->setRequestData(data, "application/atom+xml");
  request->setProperty("Item", QVariant::fromValue(item));
  
  m_gam->sendRequest(request);
  
  Q_UNUSED (partIdentifiers);
}

void ContactsResource::itemRemoved(const Akonadi::Item& item)
{
  emit status(Running, i18n("Removing contact"));
  
  QString url = Service::Addressbook::removeUrl().arg("default").arg(item.remoteId());

  KGoogleRequest *request;
  request = new KGoogleRequest(QUrl(url),
			       KGoogleRequest::Remove,
			       "Addressbook");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);
}


void ContactsResource::replyReceived(KGoogleReply* reply)
{
  switch (reply->requestType()) {
    case KGoogleRequest::FetchAll:
      contactListReceived(reply);
      break;
      
    case KGoogleRequest::Fetch:
      contactReceived(reply);
      break;
      
    case KGoogleRequest::Create:
      contactCreated(reply);
      break;
      
    case KGoogleRequest::Update:
      contactUpdated(reply);
      break;
      
    case KGoogleRequest::Remove:
      contactRemoved(reply);
      break;
  }
}

void ContactsResource::contactListReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to retrieve contacts");
    return;
  }

  Item::List contacts;
  Item::List removed;
  
  QList<KGoogleObject*> allData = reply->replyData();
  foreach (KGoogleObject* object, allData) {
    Item item;
    Object::Contact *contact = static_cast<Object::Contact*>(object);
    
    item.setRemoteId(contact->id());
    
    if (contact->deleted()) {
      removed << item;
    } else {
	PhotoJob *pJob = new PhotoJob(m_auth->accessToken(),
				      contact->photoUrl());
	pJob->setProperty("contact", QVariant::fromValue((Object::Contact::Ptr)contact));
	connect(pJob, SIGNAL(finished(KJob*)),
		this, SLOT(photoJobFinished(KJob*)));

	pJob->start();
    }
  }

  itemsRetrievedIncremental(Item::List(), removed);

  /* Store the time of this sync. Next time we will only ask for items
   * that changed or were removed since sync */
  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%S"));
  
  emit percent(100);
  emit status(Idle, "Collections synchronized");
}

void ContactsResource::contactReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to fetch contact");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }     
  Object::Contact *contact = static_cast<Object::Contact*>(data.first());
  
  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());

  if (contact->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);  
  else
    itemRetrieved(item);
  
  emit status(Idle, "Contact fetched");
}

void ContactsResource::contactCreated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::Created) {
    cancelTask("Failed to create a contact");
    return;
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }    
  Object::Contact *contact = (Object::Contact*) data.first();
    
  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());
 
  changeCommitted(item);
  
  status(Idle, "Contact created");
}

void ContactsResource::contactUpdated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to update contact");
  }
  
  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask("Failed to create a contact");
    return;
  }  
  Object::Contact *contact = static_cast<Object::Contact*>(data.first());
  
  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());
  
  changeCommitted(item);
  
  status(Idle, "Contact changed");
}

void ContactsResource::contactRemoved(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask("Failed to remove contact");
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();
  
  changeCommitted(item);

  status(Idle, "Contact removed");
}

void ContactsResource::photoJobFinished(KJob* job)
{
  PhotoJob *pJob = dynamic_cast<PhotoJob*>(job);
  
  Item item;
  Object::Contact::Ptr contact = pJob->property("contact").value<Object::Contact::Ptr>();
  KABC::Addressee addressee = *contact->toKABC();
  addressee.setPhoto(KABC::Picture(pJob->photo()));
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());
  item.setMimeType(KABC::Addressee::mimeType());
  item.setPayload<KABC::Addressee>(addressee);
  
  itemsRetrievedIncremental(Item::List() << item, Item::List());
}

void ContactsResource::updateLocalContacts()
{
  /* TODO: Implement me! */
}


AKONADI_RESOURCE_MAIN (ContactsResource)
