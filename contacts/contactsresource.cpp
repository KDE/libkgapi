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
#include "groupattribute.h"
#include "settingsdialog.h"
#include "settings.h"
#include "fetchvirtualcontactsjob.h"

#include <libkgoogle/accessmanager.h>
#include <libkgoogle/auth.h>
#include <libkgoogle/fetchlistjob.h>
#include <libkgoogle/request.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/objects/contact.h>
#include <libkgoogle/objects/contactsgroup.h>
#include <libkgoogle/services/contacts.h>

#include <qstringlist.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qbuffer.h>
#include <kdebug.h>

#include <klocalizedstring.h>
#include <kio/accessmanager.h>

#include <akonadi/attribute.h>
#include <akonadi/attributefactory.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/item.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemmodifyjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>
#include <akonadi/cachepolicy.h>
#include <akonadi/collectionfetchscope.h>
#include <akonadi/linkjob.h>
#include <akonadi/unlinkjob.h>

#include <boost/bind.hpp>

#include <kabc/addressee.h>
#include <kabc/picture.h>

using namespace Akonadi;
using namespace KGoogle;

ContactsResource::ContactsResource(const QString &id): 
  ResourceBase(id)
{
  /* Register all types we are going to use in this resource */
  qRegisterMetaType< Services::Contacts >("Contacts");
  AttributeFactory::registerAttribute< GroupAttribute >();

  setNeedsNetwork(true);
  setOnline(true);

  Auth *auth = Auth::instance();
  auth->setKWalletFolder("Akonadi Google");

  m_gam = new KGoogle::AccessManager();
  m_photoNam = new KIO::Integration::AccessManager(this);

  connect(m_gam, SIGNAL(replyReceived(KGoogle::Reply*)),
	  this, SLOT(replyReceived(KGoogle::Reply*)));

  connect(m_gam, SIGNAL(error(KGoogle::Error,QString)),
	  this, SLOT(error(KGoogle::Error,QString)));
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  connect(m_photoNam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(photoRequestFinished(QNetworkReply*)));

  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  changeRecorder()->fetchCollection(true);

  if (!Settings::self()->account().isEmpty())
    synchronize();
}

ContactsResource::~ContactsResource()
{
  delete m_photoNam;
  delete m_gam;
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

void ContactsResource::error(Error errCode, const QString &msg)
{
  cancelTask(msg);

  if ((errCode == AuthError) || (errCode == BackendNotReady)) {
    status(Broken, msg);
  }

}


void ContactsResource::configure(WId windowId)
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

void ContactsResource::retrieveItems(const Akonadi::Collection& collection)
{
  if (collection.remoteId() == m_rootCollection.remoteId()) {

    ItemFetchJob *fetchJob = new ItemFetchJob(collection, this);
    fetchJob->fetchScope().fetchFullPayload(true);
    connect(fetchJob, SIGNAL(finished(KJob*)),
            this, SLOT(initialItemsFetchJobFinished(KJob*)));

    fetchJob->start();

  } else {

    FetchVirtualContactsJob *fetchJob = new FetchVirtualContactsJob(collection, m_rootCollection);
    connect(fetchJob, SIGNAL(finished(KJob*)),
	    this, SLOT(virtualItemsFetchJobFinished(KJob*)));

    fetchJob->start();

  }
}

bool ContactsResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  Q_UNUSED (parts);

  if (item.mimeType() != KABC::Addressee::mimeType())
    return false;

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return false;
  }

  QUrl url(Services::Contacts::fetchContactUrl(account->accountName(), item.remoteId()));
  if (!Settings::self()->lastSync().isEmpty())
    url.addQueryItem("updated-min", Settings::self()->lastSync());

  KGoogle::Request *request;
  request = new KGoogle::Request(url, KGoogle::Request::Fetch, "Contacts", account);
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);

  return true;
}

void ContactsResource::retrieveCollections()
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

  request = new Request(Services::Contacts::fetchAllGroupsUrl(account->accountName()),
			KGoogle::Request::FetchAll, "Contacts", account);
  request->setProperty("isCollection", true);
  m_gam->sendRequest(request);
}

void ContactsResource::initialItemsFetchJobFinished(KJob *job)
{
  if (job->error()) {
    kDebug() << "Initial item fetch failed: " << job->errorString();
    cancelTask(i18n("Initial item fetching failed"));
    return;
  }

  QUrl url = KGoogle::Services::Contacts::fetchAllContactsUrl(Settings::self()->account());
  if (!Settings::self()->lastSync().isEmpty())
    url.addQueryItem("updated-min", Settings::self()->lastSync());

  FetchListJob *fetchJob = new FetchListJob(url, "Contacts", Settings::self()->account());
  connect(fetchJob, SIGNAL(finished(KJob*)),
    this, SLOT(contactListReceived(KJob*)));
  fetchJob->start();
}

void ContactsResource::virtualItemsFetchJobFinished(KJob *job)
{
  qDebug() << "ContactsResource::virtualItemsFetchJobFinished() " << (job->error() ? "with error" : "succesfully");

  if (job->error()) {
    cancelTask(i18n("Failed to fetch virtual contacts"));
    return;
  }

  itemsRetrievalDone();
}

void ContactsResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  Services::Contacts service;
  QByteArray data;
  KGoogle::Request *request;

  KABC::Addressee addressee = item.payload< KABC::Addressee >();
  Objects::Contact *contact = static_cast< Objects::Contact* >(&addressee);

  data = service.objectToXML(contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
			   "xmlns:gd=\"http://schemas.google.com/g/2005\" "
			   "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
	       "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
			      "term=\"http://schemas.google.com/contact/2008#contact\"/>");
  data.append("</atom:entry>");

  request = new KGoogle::Request(Services::Contacts::createContactUrl(account->accountName()),
				 Request::Create, "Contacts", account);
  request->setRequestData(data, "application/atom+xml");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);

  Q_UNUSED (collection);
}

void ContactsResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  if (!item.hasPayload< KABC::Addressee >()) {
    cancelTask(i18n("Invalid Payload"));
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

  QByteArray data;
  Services::Contacts service;
  KABC::Addressee addressee = item.payload< KABC::Addressee >();
  Objects::Contact *contact = static_cast< Objects::Contact* >(&addressee);

  data = service.objectToXML(contact);
  /* Add XML header and footer */
  data.prepend("<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" "
			   "xmlns:gd=\"http://schemas.google.com/g/2005\" "
			   "xmlns:gContact=\"http://schemas.google.com/contact/2008\">"
	       "<atom:category scheme=\"http://schemas.google.com/g/2005#kind\" "
			      "term=\"http://schemas.google.com/contact/2008#contact\"/>");
  data.append("</atom:entry>");

  KGoogle::Request *request;
  request = new KGoogle::Request(Services::Contacts::updateContactUrl(account->accountName(), item.remoteId()),
				 Request::Update, "Contacts", account);
  request->setRequestData(data, "application/atom+xml");
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);

  Q_UNUSED (partIdentifiers);
}

void ContactsResource::itemRemoved(const Akonadi::Item& item)
{

  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit status(Broken, e.what());
    return;
  }

  KGoogle::Request *request;
  request = new KGoogle::Request(Services::Contacts::removeContactUrl(account->accountName(), item.remoteId()),
				 Request::Remove, "Contacts", account);
  request->setProperty("Item", QVariant::fromValue(item));

  m_gam->sendRequest(request);
}


void ContactsResource::replyReceived(KGoogle::Reply* reply)
{
  switch (reply->requestType()) {
    case Request::FetchAll: {
      if (reply->request()->hasProperty("isCollection"))
	groupsListReceived(reply);
    } break;

    case Request::Fetch:
      contactReceived(reply);
      break;

    case Request::Create:
      contactCreated(reply);
      break;

    case Request::Update:
      contactUpdated(reply);
      break;

    case Request::Remove:
      contactRemoved(reply);
      break;

    case Request::Move:
      break;
  }

  delete reply;
}

void ContactsResource::groupsListReceived(Reply *reply)
{
  Akonadi::EntityDisplayAttribute *attr = new Akonadi::EntityDisplayAttribute();
  attr->setDisplayName(Settings::self()->account());

  Collection::List collections;

  Collection::Rights rights = Collection::CanChangeItem |
			      Collection::CanChangeItem |
			      Collection::CanDeleteItem;

  m_rootCollection.setName(identifier());
  m_rootCollection.setRemoteId(identifier());
  m_rootCollection.setParentCollection(Akonadi::Collection::root());
  m_rootCollection.setContentMimeTypes(QStringList() << Collection::mimeType()
						     << KABC::Addressee::mimeType());
  m_rootCollection.addAttribute(attr);
  m_rootCollection.setRights(rights);
  /*
  CachePolicy policy;
  policy.setInheritFromParent(false);
  policy.setSyncOnDemand(true);
  policy.setIntervalCheckTime(-1);
  m_rootCollection.setCachePolicy(policy);
*/
  collections << m_rootCollection;

  QList< KGoogle::Object* > groups = reply->replyData();
  QStringList checkedGroups = Settings::self()->groups();


  Collection myContacts;

  while (!groups.isEmpty()) {

    Objects::ContactsGroup *group = static_cast< Objects::ContactsGroup* >(groups.first());

    if (!checkedGroups.contains(group->id())) {
      groups.removeFirst();
      continue;
    }

    Collection collection;
    collection.setName(group->title());
    collection.setRemoteId(group->id());
    collection.setRights(rights);

    QStringList mime_types;
    mime_types << KABC::Addressee::mimeType();
    if (group->isSystemGroup()) {

      mime_types << Collection::mimeType();
      collection.setParentCollection(m_rootCollection);

    } else {

      collection.setParentCollection(collections.at(1));

    }

    collection.setContentMimeTypes(mime_types);

    collections << collection;
    groups.removeFirst();
  }

  Collection collection;
  collection.setName(i18n("Other Contacts"));
  collection.setRemoteId("otherContacts");
  collection.setParentCollection(m_rootCollection);
  collection.setContentMimeTypes(QStringList() << KABC::Addressee::mimeType());
  collection.setRights(rights);
  collections << collection;

  collectionsRetrieved(collections);
}


void ContactsResource::contactListReceived(KJob *job)
{
  if (job->error()) {
    cancelTask(i18n("Failed to retrieve contacts"));
    return;
  }

  Item::List removed;
  Item::List changed;

  FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);
  QList< KGoogle::Object* > objects = fetchJob->items();

  foreach (KGoogle::Object* object, objects) {

    Item item;
    Objects::Contact *contact = static_cast< Objects::Contact* >(object);

    item.setRemoteId(contact->uid());

    if (contact->deleted()) {
      removed << item;
    } else {

      item.setRemoteRevision(contact->etag());
      item.setMimeType(contact->mimeType());
      item.setPayload< KABC::Addressee >(KABC::Addressee(*contact));
      GroupAttribute *attr = item.attribute< GroupAttribute >(Entity::AddIfMissing);
      attr->setGroups(contact->groups());

      changed << item;

      fetchPhoto(item);
    }
  }

  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));

  itemsRetrievedIncremental(changed, removed);
}

void ContactsResource::contactReceived(KGoogle::Reply* reply)
{
  if (reply->error() != KGoogle::OK) {
    cancelTask(i18n("Failed to fetch contact"));
    return;
  }

  QList< KGoogle::Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create a contact"));
    return;
  }
  Objects::Contact *contact = static_cast< Objects::Contact* >(data.first());

  Item item = reply->request()->property("Item").value< Item >();
  item.setRemoteId(contact->uid());
  item.setRemoteRevision(contact->etag());

  if (contact->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);
  else {
    item.setPayload< KABC::Addressee >(KABC::Addressee(*contact));
    item.setMimeType(contact->mimeType());

    itemsRetrieved (Item::List() << item);

    fetchPhoto(item);
  }
}

void ContactsResource::contactCreated(KGoogle::Reply* reply)
{
  if (reply->error() != KGoogle::Created) {
    cancelTask(i18n("Failed to create a contact"));
    return;
  }

  QList< KGoogle::Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to create a contact"));
    return;
  }
  Objects::Contact *contact = (Objects::Contact*) data.first();

  Item item = reply->request()->property("Item").value< Item >();
  item.setRemoteId(contact->uid());
  item.setRemoteRevision(contact->etag());

  changeCommitted(item);

  item.setPayload< KABC::Addressee >(KABC::Addressee(*contact));
  ItemModifyJob *modifyJob = new ItemModifyJob(item);
  connect(modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));

  updatePhoto(item);
}

void ContactsResource::contactUpdated(KGoogle::Reply* reply)
{
  if (reply->error() != KGoogle::OK) {
    cancelTask(i18n("Failed to update contact"));
    return;
  }

  QList< KGoogle::Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Failed to update a contact"));
    return;
  }
  Objects::Contact *contact = static_cast< Objects::Contact* >(data.first());

  Item item = reply->request()->property("Item").value< Item >();
  item.setRemoteId(contact->uid());
  item.setRemoteRevision(contact->etag());

  changeCommitted(item);

  updatePhoto(item);
}

void ContactsResource::contactRemoved(KGoogle::Reply* reply)
{
  if (reply->error() != KGoogle::OK) {
    cancelTask(i18n("Failed to remove contact"));
    return;
  }

  Item item = reply->request()->property("Item").value< Item >();

  changeCommitted(item);
}

void ContactsResource::photoRequestFinished(QNetworkReply* reply)
{
  /* We care only about retrieving a contact's photo. */
  if (reply->operation() == QNetworkAccessManager::GetOperation) {
    QImage image;
    image.loadFromData(reply->readAll(), "JPG");

    Item item = reply->request().attribute(QNetworkRequest::User, QVariant()).value< Item >();

    KABC::Addressee addressee = item.payload< KABC::Addressee >();
    addressee.setPhoto(KABC::Picture(image));
    item.setPayload< KABC::Addressee >(addressee);

    ItemModifyJob *modifyJob = new ItemModifyJob(item);
    connect(modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));
  }
}

void ContactsResource::fetchPhoto(Akonadi::Item &item)
{
  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    emit cancelTask(e.what());
    return;
  }

  QString id = item.remoteId().mid(item.remoteId().lastIndexOf("/"));

  QNetworkRequest request;
  request.setUrl(Services::Contacts::photoUrl(account->accountName(), id));
  request.setRawHeader("Authorization", "OAuth "+account->accessToken().toLatin1());
  request.setRawHeader("GData-Version", "3.0");

  request.setAttribute(QNetworkRequest::User, qVariantFromValue(item));
  m_photoNam->get(request);
}


void ContactsResource::updatePhoto(Item &item)
{
  Account *account;
  try {
    Auth *auth = Auth::instance();
    account = auth->getAccount(Settings::self()->account());
  }
  catch (Exception::BaseException &e) {
    cancelTask(e.what());
    return;
  }

  QString id = item.remoteId().mid(item.remoteId().lastIndexOf("/"));

  KABC::Addressee addressee = item.payload< KABC::Addressee >();
  QNetworkRequest request;
  request.setUrl(Services::Contacts::photoUrl(account->accountName(), id));
  request.setRawHeader("Authorization", "OAuth "+account->accessToken().toLatin1());
  request.setRawHeader("GData-Version", "3.0");
  request.setRawHeader("If-Match", "*");

  if (!addressee.photo().isEmpty()) {
    request.setHeader(QNetworkRequest::ContentTypeHeader, "image/*");

    QImage image = addressee.photo().data();
    QByteArray ba;
    QBuffer buffer(&ba);
    image.save(&buffer, "JPG", 100);

    m_photoNam->put(request, ba);
  } else {
    m_photoNam->deleteResource(request);
  }
}


AKONADI_RESOURCE_MAIN (ContactsResource)
