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

#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include <KDE/KLocalizedString>
#include <KDE/Akonadi/Attribute>
#include <KDE/Akonadi/AttributeFactory>
#include <KDE/Akonadi/EntityDisplayAttribute>
#include <KDE/Akonadi/Item>
#include <KDE/Akonadi/ItemFetchJob>
#include <KDE/Akonadi/ItemFetchScope>
#include <KDE/Akonadi/ChangeRecorder>
#include <KDE/KABC/Addressee>

#include "contact.h"
#include "contactsresource.h"
#include "contactjob.h"
#include "contactcreatejob.h"
#include "contactchangejob.h"
#include "contactdeletejob.h"
#include "contactlistjob.h"
#include "settings.h"
#include "libkgoogle/authdialog.h"

using namespace Akonadi;
//using namespace Contact;

ContactsResource::ContactsResource(const QString &id): 
  ResourceBase(id)
{
  setObjectName(QLatin1String("GoogleContactsResource"));
  setNeedsNetwork(true);
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  
  resetState();
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  
  /* If the resource is ready then sync it */
  if (!Settings::self()->accessToken().isEmpty() &&
      !Settings::self()->refreshToken().isEmpty()) {
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
  resetState();
  cancelTask();
}

void ContactsResource::resetState()
{
  m_idle = true;
  m_photosCnt = 0;
  m_contactsCnt = 0;
  
  m_currentJobs.clear();
  m_existingContacts.clear();
  m_pendingContacts.clear();
}

void ContactsResource::slotAbortRequested()
{
  if (!m_idle) {
    foreach (const QPointer<KJob> &job, m_currentJobs) {
      if (job) {
	job->kill(KJob::Quietly);
      }
    }
    
    abort();
  }
}

void ContactsResource::configure(WId windowId)
{
  AuthDialog *authDialog = new AuthDialog(this, windowId);
  authDialog->setScopes(QStringList() << "https://www.google.com/m8/feeds/");
  authDialog->auth(Settings::self()->clientId(),
		   Settings::self()->clientSecret());
  if (authDialog->exec() == KDialog::Accepted) {
    Settings::self()->setAccessToken(authDialog->accessToken());
    Settings::self()->setRefreshToken(authDialog->refreshToken());
    Settings::self()->writeConfig();
    emit configurationDialogAccepted();
  } else {
    emit configurationDialogRejected();
  }
  
  delete authDialog;
  
  synchronize();
}

void ContactsResource::retrieveItems(const Akonadi::Collection& collection)
{
    if (collection.remoteId() == "googleContacts") {
      m_idle = false;
      emit status(Running, i18n("Preparing sync of contacts"));
      ItemFetchJob *fetchJob = new ItemFetchJob(collection);
      fetchJob->fetchScope().fetchFullPayload(false);
      m_currentJobs << fetchJob;
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
  
  qDebug() << item.mimeType();
  
  if (item.mimeType() == "text/directory") {
    m_idle = false;
    ContactJob *contactJob = new ContactJob(item.remoteId(),
					    Settings::self()->accessToken());
    m_currentJobs << contactJob;
    contactJob->setProperty("Item", QVariant::fromValue(item));
    connect(contactJob, SIGNAL(result(KJob*)),
	    this, SLOT(contactJobFinished(KJob*)));
    
    emit status(Running, "Fetching contact");
    contactJob->start();
  }
  
  return true;
}

void ContactsResource::retrieveCollections()
{
  Collection contacts;
  contacts.setRemoteId("googleContacts");
  contacts.setName(i18n("Google Contacts"));
  contacts.setParentCollection(Akonadi::Collection::root());
  contacts.setContentMimeTypes(QStringList() << KABC::Addressee::mimeType());
  contacts.setRights(Collection::CanDeleteItem |
		     Collection::CanCreateItem |
		     Collection::CanChangeItem);

  collectionsRetrieved(Collection::List() << contacts);
}

void ContactsResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);
  m_currentJobs.removeAll (job);
  
  if (fetchJob->error()) {
    cancelTask("Failed to fetch initial data.");
    return;
  }
  
  ContactListJob *clJob = new ContactListJob(Settings::self()->accessToken(),
					     Settings::self()->lastSync());
  m_currentJobs << clJob;
  connect (clJob, SIGNAL(result(KJob*)),
	   this, SLOT(contactListJobFinished(KJob*)));
  connect (clJob, SIGNAL(percent(KJob*,ulong)),
	   this, SLOT(emitPercent(KJob*,ulong)));
	      
  emit status(Running, i18n("Retrieving list of contacts"));
  emit percent(2);
  clJob->start();  
}

void ContactsResource::contactJobFinished(KJob* job)
{
  ContactJob *cJob = dynamic_cast<ContactJob*>(job);
  m_currentJobs.removeAll(job);
  
  if (cJob->error()) {
    cancelTask("Failed to fetch contact: " + cJob->errorText());
    return;
  }
  
  Item item;
  Contact::Contact *contact = cJob->contact();
  
  item.setMimeType(KABC::Addressee::mimeType());
  item.setPayload<KABC::Addressee>(*contact->toKABC());
  item.setRemoteId(contact->etag());

  if (contact->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);  
  else
    itemRetrieved(item);
  
  emit status(Idle, "Contact fetched");
  resetState(); 
}

void ContactsResource::contactListJobFinished(KJob* job)
{
  Q_ASSERT( m_currentJobs.indexOf(job) != -1 );

  ContactListJob * const clJob = dynamic_cast<ContactListJob*>(job);
  m_currentJobs.clear();

  if (clJob->error()) {
    qDebug() << clJob->errorText();
    cancelTask(clJob->errorText());
    return;
  }
  
  QList<Contact::Contact*> *addresses = clJob->contacts();
  QList<Item> contacts;
  QList<Item> removed;
  
  for (int i = 0; i < addresses->length(); i++) {
      Item item;
      Contact::Contact *contact = addresses->at(i);

      item.setRemoteId(contact->etag());
      
      if (contact->deleted()) {
	removed << item;
      } else {
	item.setMimeType(KABC::Addressee::mimeType());
	item.setPayload<KABC::Addressee>(*contact->toKABC());
	contacts << item;
      }
  }
  
  itemsRetrievedIncremental(contacts, removed);
  
  /* Store the time of this sync. Next time we will only ask for items
   * that changed or were removed since sync */
  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%S"));
  
  emit percent(100);
  emit status(Idle, "Collections synchronized");
  resetState();
}


void ContactsResource::photoJobFinished(KJob* job)
{
  /* TODO: Implement me! */
  
  Q_UNUSED (job);
}

void ContactsResource::updateLocalContacts()
{
  /* TODO: Implement me! */
}

void ContactsResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;

  status(Running, "Creating contact...");
  
  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  ContactCreateJob *ccJob = new ContactCreateJob (addressee, Settings::self()->accessToken());
  ccJob->setProperty("Item", QVariant::fromValue(item));
  connect (ccJob, SIGNAL(finished(KJob*)),
	   this, SLOT(addJobFinished(KJob*)));
  ccJob->start();
  
  Q_UNUSED (collection);
}

void ContactsResource::addJobFinished(KJob* job)
{
  if (job->error()) {
    qDebug() << job->errorString();
    cancelTask(job->errorString());
    return;
  }

  Contact::Contact *contact = dynamic_cast<ContactCreateJob*>(job)->newContact();
  Akonadi::Item newItem;
  
  newItem.setRemoteId(contact->etag());
  newItem.setMimeType(KABC::Addressee::mimeType());
  newItem.setPayload<KABC::Addressee>(*contact->toKABC());

  changeCommitted(newItem);
  
  status(Idle, "Contact created");
  resetState();
}

void ContactsResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  if (!item.hasPayload<KABC::Addressee>())
    return;
  
  status(Running, "Updating contact...");
  
  KABC::Addressee addressee = item.payload<KABC::Addressee>();
  ContactChangeJob *ccJob = new ContactChangeJob(addressee, item.remoteId(), Settings::self()->accessToken());
  ccJob->setProperty("Item", QVariant::fromValue(item));
  connect (ccJob, SIGNAL(finished(KJob*)),
	   this, SLOT(changeJobFinished(KJob*)));
  ccJob->start();
  
  Q_UNUSED (partIdentifiers);
}

void ContactsResource::changeJobFinished(KJob* job)
{
  if (job->error()) {
    qDebug() << job->errorString();
    cancelTask(job->errorString());
  }
  
  ContactChangeJob *ccJob = dynamic_cast<ContactChangeJob*>(job);
  
  Contact::Contact *contact = ccJob->newContact();
  Akonadi::Item oldItem = ccJob->property("Item").value<Item>();
  Akonadi::Item newItem;

  newItem.setRemoteId(contact->etag());
  newItem.setMimeType(KABC::Addressee::mimeType());
  newItem.setPayload<KABC::Addressee>(*contact->toKABC());
  
  changeCommitted(newItem);
  
  status(Idle, "Contact changed");
  resetState();
}

void ContactsResource::itemRemoved(const Akonadi::Item& item)
{
  QString uid = item.remoteId();
  
  emit status(Running, i18n("Removing contact"));
  
  ContactDeleteJob *dJob = new ContactDeleteJob(uid, Settings::self()->accessToken());
  dJob->setProperty("Item", QVariant::fromValue(item));
  connect (dJob, SIGNAL(finished(KJob*)),
	   this, SLOT(removeJobFinished(KJob*)));
  dJob->start();
  m_currentJobs.append(dJob);
}

void ContactsResource::removeJobFinished(KJob* job)
{
  m_currentJobs.removeAll(job);
  
  if (job->error()) {
    qDebug() << job->errorString();
    cancelTask(job->errorString());
    resetState();
    return;
  }

  Akonadi::Item item = dynamic_cast<ContactDeleteJob*>(job)->property("Item").value<Item>();
  changeCommitted(item);
  resetState();
}

AKONADI_RESOURCE_MAIN (ContactsResource)
