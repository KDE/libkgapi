/*
    Akonadi Google - Tasks Resource
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


#ifndef TASKSRESOURCE_H
#define TASKSRESOURCE_H

#include <Akonadi/AgentBase>
#include <Akonadi/ResourceBase>

#include <KDE/KJob>

namespace KGoogle {
  class KGoogleAccessManager;
  class KGoogleAuth;
  class KGoogleReply;
}

using namespace KGoogle;

class TasksResource : public Akonadi::ResourceBase, public Akonadi::AgentBase::Observer
{

  Q_OBJECT
  public:
    TasksResource(const QString &id);
    ~TasksResource();

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
   
    void replyReceived(KGoogleReply *reply);    
    
    void initialItemFetchJobFinished(KJob *job);
    void taskListReceived(KGoogleReply *reply);
    void taskReceived(KGoogleReply *reply);
    void taskCreated(KGoogleReply *reply);
    void taskUpdated(KGoogleReply *reply);
    void taskRemoved(KGoogleReply *reply);

  private:
    void abort();
    
    KGoogleAccessManager *m_gam;
    KGoogleAuth *m_auth;
};

#endif // TASKSRESOURCE_H
