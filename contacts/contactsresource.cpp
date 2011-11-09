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

#include "libkgoogle/kgoogleaccessmanager.h"
#include "libkgoogle/kgoogleauth.h"
#include "libkgoogle/kgooglerequest.h"
#include "libkgoogle/kgooglereply.h"
#include "libkgoogle/objects/contact.h"
#include "libkgoogle/services/addressbook.h"

#include <qstringlist.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qbuffer.h>
#include <kdebug.h>

#include <klocalizedstring.h>
#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>
#include <akonadi/cachepolicy.h>

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
  setOnline(false);  

  m_auth = new KGoogleAuth(Settings::self()->clientId(),
			   Settings::self()->clientSecret(),
			   Service::Addressbook::scopeUrl());
  connect(m_auth, SIGNAL(tokensRecevied(QString,QString)),
	  this, SLOT(tokensReceived()));
  
  m_gam = new KGoogleAccessManager(m_auth);
  
  m_photoNam = new QNetworkAccessManager();
  
  connect(m_gam, SIGNAL(replyReceived(KGoogleReply*)),
	  this, SLOT(replyReceived(KGoogleReply*)));
  connect(m_gam, SIGNAL(requestFinished(KGoogleRequest*)),
	  this, SLOT(commitItemsList()));
  connect(m_gam, SIGNAL(error(QString,int)),
	  this, SLOT(slotGAMError(QString,int)));
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  connect(m_photoNam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(photoRequestFinished(QNetworkReply*)));
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
}

ContactsResource::~ContactsResource()
{
  delete m_photoNam;
  delete m_gam;
  delete m_auth;
}

void ContactsResource::aboutToQuit()
{
  slotAbortRequested();
}

void ContactsResource::abort()
{
  cancelTask(i18n("Aborted"));
}

void ContactsResource::slotAbortRequested()
{
  abort();
}

void ContactsResource::slotGAMError(const QString &msg, const int errorCode)
{
  cancelTask(msg);

  Q_UNUSED(errorCode);
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
    QUrl url(Service::Addressbook::fetchUrl().arg("default").arg(item.remoteId()));
    url.addQueryItem("alt", "json");
    if (!Settings::self()->lastSync().isEmpty())
      url.addQueryItem("updated-min", Settings::self()->lastSync());

    KGoogleRequest *request;
    request = new KGoogleRequest(url,
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

  if (Settings::self()->refreshAddressbook()) {
    Akonadi::CachePolicy policy;
    policy.setInheritFromParent(true);
    policy.setIntervalCheckTime(Settings::self()->refreshInterval());
    policy.setSyncOnDemand(true);
    contacts.setCachePolicy(policy);
  }

  collectionsRetrieved(Collection::List() << contacts);

  /* Use user-friendly name in resource configuration dialog */
  setAgentName(i18n("Google Contacts"));
}

void ContactsResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);

  if (fetchJob->error()) {
    cancelTask(i18n("Failed to fetch initial data."));
    return;
  }

  QUrl url(Service::Addressbook::fetchAllUrl().arg("default"));
  url.addQueryItem("alt", "json");
  if (!Settings::self()->lastSync().isEmpty())
    url.addQueryItem("updated-min", Settings::self()->lastSync());

  KGoogleRequest *request = new KGoogleRequest(url,
					       KGoogleRequest::FetchAll,
					       "Addressbook");
  m_gam->sendRequest(request);

  emit status(Running, i18n("Retrieving list of contacts"));
}

void ContactsResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;

  status(Running, i18n("Creating contact..."));

  Service::Addressbook service;
  QString url = Service::Addressbook::createUrl().arg("default");
  QByteArray data;
  KGoogleRequest *request;

  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  Object::Contact *contact = static_cast<Object::Contact*>(&addressee);
  data = service.objectToXML(contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" xmlns:gd=\"http://schemas.google.com/g/2005\" xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
	       "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/contact/2008#contact\"/>");
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
  if (!item.hasPayload<KABC::Addressee>()) {
    cancelTask(i18n("Invalid Payload"));
    return;
  }

  status(Running, i18n("Updating contact..."));

  QString url = Service::Addressbook::updateUrl().arg("default").arg(item.remoteId());
  QByteArray data;
  Service::Addressbook service;
  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  Object::Contact *contact = static_cast<Object::Contact*>(&addressee);
  data = service.objectToXML(contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" xmlns:gd=\"http://schemas.google.com/g/2005\" xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
	       "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/contact/2008#contact\"/>");
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
    cancelTask(i18n("Failed to retrieve contacts"));
    return;
  }

  Item::List removed;
  Item::List changed;

  QList<KGoogleObject*> allData = reply->replyData();
  foreach (KGoogleObject* object, allData) {
    Item item;
    Object::Contact *contact = static_cast<Object::Contact*>(object);
    item.setRemoteId(contact->id());

    if (contact->deleted()) {
      m_removedItems << item;
    } else {
      item.setRemoteRevision(contact->etag());
      item.setMimeType(contact->mimeType());
      item.setPayload<KABC::Addressee>(KABC::Addressee(*contact));
      fetchPhoto(&item, contact->photoUrl().toString(), (allData.last() == object));
    }
  }
}

void ContactsResource::contactReceived(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to fetch contact"));
    return;
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create a contact"));
    return;
  }
  Object::Contact *contact = static_cast<Object::Contact*>(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());

  if (contact->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);  
  else {
    item.setPayload<KABC::Addressee>(KABC::Addressee(*contact));
    item.setMimeType(contact->mimeType());
    fetchPhoto(&item, contact->photoUrl().toString(), false);  
  }

  emit percent(100);
  emit status(Idle, i18n("Contact fetched"));
  taskDone();
}

void ContactsResource::contactCreated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::Created) {
    cancelTask(i18n("Failed to create a contact"));
    return;
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create a contact"));
    return;
  }
  Object::Contact *contact = (Object::Contact*) data.first();

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());

  updatePhoto(&item);

  changeCommitted(item);

  status(Idle, i18n("Contact created"));
  taskDone();
}

void ContactsResource::contactUpdated(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to update contact"));
    return;
  }

  QList<KGoogleObject*> data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to update a contact"));
    return;
  }
  Object::Contact *contact = static_cast<Object::Contact*>(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(contact->id());
  item.setRemoteRevision(contact->etag());

  updatePhoto(&item);

  changeCommitted(item);

  status(Idle, i18n("Contact changed"));
  taskDone();
}

void ContactsResource::contactRemoved(KGoogleReply* reply)
{
  if (reply->error() != KGoogle::KGoogleReply::OK) {
    cancelTask(i18n("Failed to remove contact"));
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();

  changeCommitted(item);

  status(Idle, i18n("Contact removed"));
  taskDone();
}

void ContactsResource::photoRequestFinished(QNetworkReply* reply)
{
  /* We care only about retrieving a contact's photo. */
  if (reply->operation() == QNetworkAccessManager::GetOperation) {
    QImage image;
    image.loadFromData(reply->readAll(), "JPG");

    Item item = reply->request().attribute(QNetworkRequest::User, QVariant()).value<Item>();

    KABC::Addressee addressee = item.payload<KABC::Addressee>();
    addressee.setPhoto(KABC::Picture(image));
    item.setPayload<KABC::Addressee>(addressee);

    m_changedItems << item;
  }
}

void ContactsResource::fetchPhoto(Akonadi::Item *item, const QString &photoUrl, const bool isLastItem)
{
  QString photoId = photoUrl.mid(photoUrl.lastIndexOf("/")+1);

  QNetworkRequest request;
  request.setUrl(QUrl("https://www.google.com/m8/feeds/photos/media/default/"+photoId));
  request.setRawHeader("Authorization", "OAuth "+m_auth->accessToken().toLatin1());
  request.setRawHeader("GData-Version", "3.0");  

  request.setAttribute(QNetworkRequest::User, QVariant::fromValue(*item));
  request.setAttribute(QNetworkRequest::UserMax, QVariant(isLastItem));
  m_photoNam->get(request);
}


void ContactsResource::updatePhoto(Item* item)
{
  KABC::Addressee addressee = item->payload<KABC::Addressee>();
  QNetworkRequest request;
  request.setUrl(QUrl("https://www.google.com/m8/feeds/photos/media/default/"+item->remoteId()));
  request.setRawHeader("Authorization", "OAuth "+m_auth->accessToken().toLatin1());
  request.setRawHeader("GData-Version", "3.0");  
  request.setRawHeader("If-Match", "*");

  if (!addressee.photo().isEmpty()) {
    request.setHeader(QNetworkRequest::ContentTypeHeader, "Image/*");

    QImage image = addressee.photo().data();
    QByteArray ba;
    QBuffer buffer(&ba);
    image.save(&buffer, "JPG", 100);

    m_photoNam->put(request, ba);
  } else {
    m_photoNam->deleteResource(request);
  }
}

void ContactsResource::tokensReceived()
{
  if (m_auth->accessToken().isEmpty()) {
    emit status(Broken, i18n("Failed to fetch tokens"));
  } else {
    setOnline(true);
    synchronize();
  }
}

void ContactsResource::commitItemsList()
{
  if (Settings::self()->lastSync().isEmpty())
    itemsRetrieved(m_changedItems);
  else
    itemsRetrievedIncremental(m_changedItems, m_removedItems);

  m_changedItems.clear();
  m_removedItems.clear();

  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));

  emit percent(100);
  emit status(Idle, i18n("Collections synchronized"));
  taskDone();
}



AKONADI_RESOURCE_MAIN (ContactsResource)
