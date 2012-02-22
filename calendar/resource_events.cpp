/*
 *    Akonadi Google - Calendar Resource
 *    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "calendarresource.h"
#include "settings.h"

#include <libkgoogle/fetchlistjob.h>
#include <libkgoogle/reply.h>
#include <libkgoogle/request.h>
#include <libkgoogle/objects/event.h>
#include <libkgoogle/objects/calendar.h>
#include <libkgoogle/services/calendar.h>

#include <klocalizedstring.h>

#include <akonadi/itemmodifyjob.h>
#include <akonadi/entitydisplayattribute.h>
#include <akonadi/collectionmodifyjob.h>

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

void CalendarResource::calendarsReceived(KJob *job)
{
  if (job->error()) {
    emit status(Broken, i18n("Failed to fetch calendars: %1").arg(job->errorString()));
    cancelTask();
    return;
  }

  FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);

  QStringList calendars = Settings::self()->calendars();

  QList< KGoogle::Object *> allData = fetchJob->items();
  foreach (KGoogle::Object* replyData, allData) {

      Objects::Calendar *calendar = static_cast< Objects::Calendar* >(replyData);

      if (!calendars.contains(calendar->uid()))
        continue;

      Collection collection;
      collection.setRemoteId(calendar->uid());
      collection.setParentCollection(m_collections.first());
      collection.setContentMimeTypes(QStringList() << Event::eventMimeType());
      collection.setName(calendar->title());
      collection.setRights(Collection::AllRights);

      EntityDisplayAttribute *attr = new EntityDisplayAttribute;
      attr->setDisplayName(calendar->title());
      attr->setIconName("text-calendar");
      collection.addAttribute(attr);

      m_collections.append(collection);

  }

  m_fetchedCalendars = true;

  if (m_fetchedTaskLists && m_fetchedCalendars) {
    collectionsRetrieved(m_collections);
    m_collections.clear();

    m_fetchedCalendars = false;
    m_fetchedTaskLists = false;
  }
}

void CalendarResource::eventReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask();
    emit status(Broken, i18n("Failed to fetch event: %1").arg(reply->errorString()));
    return;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return;
  }

  Objects::Event *event = static_cast< Objects::Event* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(event->uid());
  item.setRemoteRevision(event->etag());
  item.setMimeType(Event::eventMimeType());
  item.setPayload< EventPtr >(EventPtr(event));

  if (event->deleted())
    itemsRetrievedIncremental(Item::List(), Item::List() << item);
  else
    itemRetrieved(item);
}

void CalendarResource::eventsReceived(KJob *job)
{
  if (job->error()) {
    cancelTask(i18n("Failed to fetch events: %1").arg(job->errorString()));
    return;
  }

  FetchListJob *fetchJob = dynamic_cast< FetchListJob* >(job);
  Collection collection = fetchJob->property("collection").value< Collection >();

  Item::List removed;
  Item::List changed;

  QList< Object *> allData = fetchJob->items();
  foreach (Object* replyData, allData) {

    Objects::Event *event = static_cast< Objects::Event* >(replyData);

    Item item;
    item.setRemoteId(event->uid());
    item.setRemoteRevision(event->etag());
    item.setPayload< EventPtr >(EventPtr(event));
    item.setMimeType(Event::eventMimeType());
    item.setParentCollection(collection);

    if (event->deleted())
      removed << item;
    else
      changed << item;

  }

  itemsRetrievedIncremental(changed, removed);

  collection.setRemoteRevision(QString::number(KDateTime::currentUtcDateTime().toTime_t()));
  CollectionModifyJob *modifyJob = new CollectionModifyJob(collection, this);
  modifyJob->setAutoDelete(true);
  modifyJob->start();
}

void CalendarResource::eventCreated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to create a new event: %1").arg(reply->errorString()));
    return;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return;
  }

  Objects::Event *event = static_cast< Objects::Event* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(event->uid());
  item.setRemoteRevision(event->etag());
  item.setMimeType(Event::eventMimeType());
  item.setPayload< EventPtr >(EventPtr(event));
  item.setParentCollection(reply->request()->property("Collection").value<Collection>());

  changeCommitted(item);

  Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(item);
  connect (modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));
}

void CalendarResource::eventUpdated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to update an event: %1").arg(reply->errorString()));
    return;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return;
  }

  Objects::Event *event = static_cast< Objects::Event* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteRevision(event->etag());
  item.setPayload< EventPtr >(EventPtr(event));

  changeCommitted(item);
}

void CalendarResource::eventRemoved(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to delete event: %1").arg(reply->errorString()));
    return;
  }

  Item item = reply->request()->property("Item").value<Item>();
  changeCommitted(item);
}

