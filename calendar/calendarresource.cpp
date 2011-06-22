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

#include <Akonadi/Collection>
#include <Akonadi/ChangeRecorder>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/EntityDisplayAttribute>

#include <KDE/KCalCore/Event>
#include <KDE/KCalCore/Calendar>
#include <KDE/KLocalizedString>

#include <QtCore/QPointer>

#include "calendarresource.h"
#include "eventlistjob.h"
#include "eventcreatejob.h"
#include "eventupdatejob.h"
#include "eventjob.h"
#include "event.h"
#include "settingsdialog.h"
#include "settings.h"

using namespace Akonadi;

CalendarResource::CalendarResource(const QString &id): 
  ResourceBase(id)
{
  setObjectName(QLatin1String("GoogleCalendarResource"));
  setNeedsNetwork(true);
  
  connect(this, SIGNAL(abortRequested()),
	  this, SLOT(slotAbortRequested()));
  
  resetState();
  
  changeRecorder()->fetchCollection(true);
  changeRecorder()->itemFetchScope().fetchFullPayload(true);
  
  /* If the resource is ready then sync it */
  if (!Settings::self()->calendarId().isEmpty() &&
      !Settings::self()->accessToken().isEmpty() &&
      !Settings::self()->refreshToken().isEmpty()) {
    synchronize();
  }
  
}

CalendarResource::~CalendarResource()
{
}

void CalendarResource::abort()
{
  resetState();
  cancelTask();
}


void CalendarResource::resetState()
{
  m_currentJobs.clear();
}

void CalendarResource::slotAbortRequested()
{
  foreach (const QPointer<KJob> &job, m_currentJobs) {
    if (job) {
      job->kill(KJob::Quietly);
    }
  }
    
  
}


void CalendarResource::configure(WId windowId)
{
  SettingsDialog *settingsDialog = new SettingsDialog(windowId);
  if (settingsDialog->exec() == KDialog::Accepted) {
    emit configurationDialogAccepted();
  } else {
    emit configurationDialogRejected();
  }
  
  delete settingsDialog;
  
  synchronize();  
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

void CalendarResource::retrieveItems(const Akonadi::Collection& collection)
{
  emit status(Running, i18n("Preparing to synchronize calendar"));
  ItemFetchJob *fetchJob = new ItemFetchJob(collection);
  fetchJob->fetchScope().fetchFullPayload(false);
  m_currentJobs << fetchJob;
  connect (fetchJob, SIGNAL(result(KJob*)),
	   this, SLOT(initialItemFetchJobFinished(KJob*)));
  fetchJob->start();
}

void CalendarResource::initialItemFetchJobFinished(KJob* job)
{
  ItemFetchJob *fetchJob = dynamic_cast<ItemFetchJob*>(job);
  m_currentJobs.removeAll (job);
  
  if (fetchJob->error()) {
    cancelTask("Failed to fetch initial data.");
    return;
  }
  
  EventListJob *elJob = new EventListJob(Settings::self()->accessToken(),
					 Settings::self()->lastSync(),
					 Settings::self()->calendarId());
  m_currentJobs << elJob;
  connect (elJob, SIGNAL(result(KJob*)),
	   this, SLOT(eventListJobFinished(KJob*)));
  emit status(Running, i18n("Retrieving events"));
  emit percent(2);
  elJob->start();  
}

void CalendarResource::eventListJobFinished(KJob* job)
{
  Q_ASSERT( m_currentJobs.indexOf(job) != -1 );

  EventListJob *elJob = dynamic_cast<EventListJob*>(job);
  m_currentJobs.clear();

  if (elJob->error()) {
    qDebug() << elJob->errorText();
    cancelTask(elJob->errorText());
    return;
  }
  
  Event::Event::List events = elJob->events();
  QList<Item> new_items;
  QList<Item> removed;
  
  for (int i = 0; i < events.length(); i++) {
      Item item;
      Event::Event event = events.at(i);

      item.setRemoteId(event.uid());
      if (event.deleted()) {
	removed << item;
      } else {
	item.setMimeType(event.mimeType());
	item.setPayload<KCalCore::Event::Ptr>((KCalCore::Event::Ptr) event.clone());
	new_items << item;
      }
  }
  
  itemsRetrievedIncremental(new_items, removed);
  
  /* Store the time of this sync. Next time we will only ask for items
   * that changed or were removed since sync */
  Settings::self()->setLastSync(KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%S"));
  
  emit percent(100);
  emit status(Idle, "Collections synchronized");
  resetState();
}



bool CalendarResource::retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts)
{
  EventJob *eJob = new EventJob(Settings::self()->accessToken(),
				Settings::self()->calendarId(),
				item.remoteId());
  m_currentJobs << eJob;
  eJob->setProperty("Item", QVariant::fromValue(item));
  connect(eJob, SIGNAL(result(KJob*)),
	  this, SLOT(eventJobFinished(KJob*)));
    
  emit status(Running, "Fetching event");
  eJob->start();
  
  return true;
  
  Q_UNUSED (parts);
}

void CalendarResource::eventJobFinished(KJob* job)
{
  EventJob *eJob = dynamic_cast<EventJob*>(job);
  m_currentJobs.removeAll(job);
  
  if (eJob->error()) {
    cancelTask("Failed to fetch event: " + eJob->errorText());
    return;
  }
  
  Item item;
  Event::Event* event = eJob->getEvent();
  item.setMimeType(event->mimeType());
  item.setPayload<KCalCore::Event::Ptr>((KCalCore::Event::Ptr) event->clone());
  
  emit status(Idle, "Event fetched");
  itemRetrieved(item);
  resetState(); 
}


void CalendarResource::itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection)
{
  if (!item.hasPayload<KCalCore::Event::Ptr>())
    return;

  status(Running, "Creating event...");
  
  KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();
  Event::Event *d_event = new Event::Event(event.data());
  EventCreateJob *ecJob = new EventCreateJob(d_event,
					     Settings::self()->calendarId(),
					     Settings::self()->accessToken());
  ecJob->setProperty("Item", QVariant::fromValue(item));
  connect (ecJob, SIGNAL(finished(KJob*)),
	   this, SLOT(addJobFinished(KJob*)));
  ecJob->start();

  Q_UNUSED (collection)
}

void CalendarResource::addJobFinished(KJob* job)
{
  if (job->error()) {
    qDebug() << job->errorString();
    cancelTask(job->errorString());
    return;
  }

  EventCreateJob *ecJob = dynamic_cast<EventCreateJob*>(job);

  KCalCore::Event* event = ecJob->newEvent();
  Akonadi::Item oldItem = ecJob->property("Item").value<Akonadi::Item>();
  Akonadi::Item newItem;
  
  newItem.setRemoteId(event->uid());
  newItem.setRemoteRevision(event->uid());
  newItem.setMimeType(event->mimeType());
  newItem.setPayload<KCalCore::Event::Ptr>((KCalCore::Event::Ptr)event->clone());

  changeCommitted(newItem);
  
  status(Idle, "Event created");
  resetState();  
}


void CalendarResource::itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers)
{
  if (!item.hasPayload<KCalCore::Event::Ptr>())
    return;
  
  status(Running, "Updating evnet...");
  
  KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();
  Event::Event *d_event = new Event::Event(event.data());
  EventUpdateJob *upJob = new EventUpdateJob(d_event,
					     Settings::self()->calendarId(),
					     Settings::self()->accessToken());
  upJob->setProperty("Item", QVariant::fromValue(item));
  connect(upJob, SIGNAL(finished(KJob*)),
	  this, SLOT(updateJobFinished(KJob*)));
  upJob->start();
  
  Q_UNUSED(partIdentifiers)
}

void CalendarResource::updateJobFinished(KJob* job)
{
  if (job->error()) {
    qDebug() << job->errorString();
    cancelTask(job->errorString());
    return;
  }
  
  EventUpdateJob *upJob = dynamic_cast<EventUpdateJob*>(job);
  
  KCalCore::Event *event = upJob->newEvent();
  Akonadi::Item newItem;
  newItem.setRemoteId(event->uid());
  newItem.setRemoteRevision(event->uid());
  newItem.setMimeType(event->mimeType());
  newItem.setPayload<KCalCore::Event::Ptr>((KCalCore::Event::Ptr)event->clone());
  
  changeCommitted(newItem);
  
  status(Idle, "Event updated");
  resetState();
}


void CalendarResource::itemRemoved(const Akonadi::Item& item)
{
  /* TODO: Implement me */
  
  Q_UNUSED(item);
}

AKONADI_RESOURCE_MAIN (CalendarResource)