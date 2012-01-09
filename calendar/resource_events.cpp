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

#include <libkgoogle/reply.h>
#include <libkgoogle/request.h>
#include <libkgoogle/objects/event.h>
#include <libkgoogle/objects/calendar.h>
#include <libkgoogle/services/calendar.h>

#include <klocalizedstring.h>

#include <akonadi/itemmodifyjob.h>
#include <akonadi/entitydisplayattribute.h>

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

bool CalendarResource::calendarsReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    emit status(Broken, i18n("Failed to fetch calendars: %1").arg(reply->errorString()));
    return false;
  }

  Item::List events;
  Item::List removed;
  Item::List changed;

  QStringList calendars = Settings::self()->calendars();

  QList< KGoogle::Object *> allData = reply->replyData();
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

  return true;
}

bool CalendarResource::commitCalendars(KGoogle::Request *request)
{
  QString collectionId = request->property("collectionId").toString();

  m_fetchedCalendars = true;

  if (m_fetchedCalendars && m_fetchedTaskLists) {

    collectionsRetrieved(m_collections);

    m_collections.clear();
    m_fetchedCalendars = false;
    m_fetchedTaskLists = false;

    return true;
  }

  return false;
}

bool CalendarResource::eventReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    emit status(Broken, i18n("Failed to fetch event: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
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

  emit status(Idle, i18n("Event fetched"));

  return true;
}

bool CalendarResource::eventsReceived(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to fetch events: %1").arg(reply->errorString()));
    return false;
  }

  Item::List events;
  Item::List removed;
  Item::List changed;

  QList< Object *> allData = reply->replyData();
  foreach (Object* replyData, allData) {

    Objects::Event *event = static_cast< Objects::Event* >(replyData);

    Item item;
    item.setRemoteId(event->uid());
    item.setRemoteRevision(event->etag());
    item.setPayload< EventPtr >(EventPtr(event));
    item.setMimeType(Event::eventMimeType());
    item.setParentCollection(reply->request()->property("Collection").value<Collection>());

    if (event->deleted())
      m_removedEvents << item;
    else
      m_changedEvents << item;

  }

  /* Don't emit taskDone() - will be emitted after commitEvents() is called. */
  return false;
}

bool CalendarResource::eventCreated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to create a new event: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
  }

  Objects::Event *event = static_cast< Objects::Event* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteId(event->uid());
  item.setRemoteRevision(event->etag());
  item.setMimeType(Event::eventMimeType());
  item.setPayload< EventPtr >(EventPtr(event));
  item.setParentCollection(reply->request()->property("Collection").value<Collection>());

  emit status(Idle, i18n("Event created"));

  changeCommitted(item);

  Akonadi::ItemModifyJob *modifyJob = new Akonadi::ItemModifyJob(item);
  connect (modifyJob, SIGNAL(finished(KJob*)), modifyJob, SLOT(deleteLater()));

  return true;
}

bool CalendarResource::eventUpdated(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to update an event: %1").arg(reply->errorString()));
    return false;
  }

  QList< Object* > data = reply->replyData();
  if (data.length() != 1) {
    kWarning() << "Server send " << data.length() << "items, which is not OK";
    cancelTask(i18n("Expected a single item, server sent %1 items.").arg(data.length()));
    return false;
  }

  Objects::Event *event = static_cast< Objects::Event* >(data.first());

  Item item = reply->request()->property("Item").value<Item>();
  item.setRemoteRevision(event->etag());
  item.setPayload< EventPtr >(EventPtr(event));

  emit status(Idle, i18n("Event updated"));

  changeCommitted(item);

  return true;
}

bool CalendarResource::eventRemoved(KGoogle::Reply *reply)
{
  if (reply->error() != OK) {
    cancelTask(i18n("Failed to delete event: %1").arg(reply->errorString()));
    return false;
  }

  Item item = reply->request()->property("Item").value<Item>();
  changeCommitted(item);

  emit status(Idle, i18n("Event deleted"));

  return true;
}

bool CalendarResource::commitEvents(KGoogle::Request *request)
{
  Collection collection = request->property("Collection").value< Collection >();
  QString collectionId = collection.remoteId();
  bool updated = false;

  QStringList calendars = Settings::self()->calendarSync();
  foreach (const QString &calendar, calendars) {

    if (calendar.startsWith(collectionId + ',')) {
      itemsRetrievedIncremental(m_changedEvents, m_removedEvents);
      calendars.removeAll(calendar);
      calendars.append(collectionId + ',' + KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));
      updated = true;
      break;
    }

  }

  if (updated == false) {
    itemsRetrieved(m_changedEvents);
    calendars.append(collectionId + ',' + KDateTime::currentUtcDateTime().toString("%Y-%m-%dT%H:%M:%SZ"));
  }

  Settings::self()->setCalendarSync(calendars);
  Settings::self()->writeConfig();

  m_changedEvents.clear();
  m_removedEvents.clear();

  emit percent(100);
  emit status(Idle, i18n("Collections synchronized"));

  taskDone();

  return true;
}
