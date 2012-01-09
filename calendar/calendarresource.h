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


#ifndef CALENDARRESOURCE_H
#define CALENDARRESOURCE_H

#include <akonadi/agentbase.h>
#include <akonadi/resourcebase.h>

#include <akonadi/item.h>
#include <akonadi/collection.h>

#include <libkgoogle/common.h>

namespace KGoogle {
  class AccessManager;
  class Account;
  class Reply;
  class Request;
}

using namespace KGoogle;

class CalendarResource : public Akonadi::ResourceBase, public Akonadi::AgentBase::Observer
{

  Q_OBJECT
  public:
    CalendarResource(const QString &id);
    ~CalendarResource();

    void configure(WId windowId);


  public Q_SLOTS:
    void retrieveCollections();
    void retrieveItems(const Akonadi::Collection& collection);
    bool retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts);

    void itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection);
    void itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers);
    void itemRemoved(const Akonadi::Item& item);

  protected:
    void aboutToQuit();

  private Q_SLOTS:
    void error(const KGoogle::Error, const QString&);
    void slotAbortRequested();

    void cachedItemsRetrieved(KJob *job);
    void replyReceived(KGoogle::Reply *reply);
    void commitItemsList(KGoogle::Request *request);

    void itemsReceived(KGoogle::Reply *reply);
    void itemReceived(KGoogle::Reply *reply);
    void itemCreated(KGoogle::Reply *reply);
    void itemUpdated(KGoogle::Reply *reply);
    void itemRemoved(KGoogle::Reply *reply);

    bool taskReceived(KGoogle::Reply *reply);
    bool tasksReceived(KGoogle::Reply *reply);
    bool taskListReceived(KGoogle::Reply *reply);
    bool taskCreated(KGoogle::Reply *reply);
    bool taskUpdated(KGoogle::Reply *reply);
    bool taskRemoved(KGoogle::Reply *reply);
    bool commitTaskLists(KGoogle::Request *request);
    bool commitTasks(KGoogle::Request *request);

    bool eventReceived(KGoogle::Reply *reply);
    bool eventsReceived(KGoogle::Reply *reply);
    bool calendarsReceived(KGoogle::Reply *reply);
    bool eventCreated(KGoogle::Reply *reply);
    bool eventUpdated(KGoogle::Reply *reply);
    bool eventRemoved(KGoogle::Reply *reply);
    bool commitCalendars(KGoogle::Request *reply);
    bool commitEvents(KGoogle::Request *reply);

  private:
    void abort();

    AccessManager *m_gam;

    Akonadi::Item::List m_changedEvents;
    Akonadi::Item::List m_changedTodos;
    Akonadi::Item::List m_removedEvents;
    Akonadi::Item::List m_removedTodos;

    Akonadi::Collection::List m_collections;
    bool m_fetchedCalendars;
    bool m_fetchedTaskLists;
};

#endif // CALENDARRESOURCE_H
