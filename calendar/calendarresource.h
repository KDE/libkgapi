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

#include <Akonadi/AgentBase>
#include <Akonadi/ResourceBase>

#include <akonadi/item.h>

namespace KGoogle {
  class KGoogleAccessManager;
  class KGoogleAuth;
  class KGoogleReply;
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
    void slotAbortRequested();
    void tokensReceived();
   
    void replyReceived(KGoogleReply *reply);
    void commitItemsList();
    
    void initialItemFetchJobFinished(KJob *job);
    void eventListReceived(KGoogleReply *reply);
    void eventReceived(KGoogleReply *reply);
    void eventCreated(KGoogleReply *reply);
    void eventUpdated(KGoogleReply *reply);
    void eventRemoved(KGoogleReply *reply);

    
  private:
    void abort();
    
    KGoogleAccessManager *m_gam;
    KGoogleAuth *m_auth;
    
    Akonadi::Item::List m_changedItems;
    Akonadi::Item::List m_removedItems;
};

#endif // CALENDARRESOURCE_H
