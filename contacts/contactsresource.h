/*
    Akonadi Google - Contacts Resource
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


#ifndef CONTACTSRESOURCE_H
#define CONTACTSRESOURCE_H

#include <QtCore/QPointer>
#include <QtCore/QMap>
#include <QtCore/QList>

#include <KDE/KDateTime>
#include <KDE/Akonadi/ResourceBase>
#include <KDE/Akonadi/AgentBase>
#include <KDE/Akonadi/Collection>
#include <KDE/Akonadi/Item>

class ContactsResource: public Akonadi::ResourceBase,
			public Akonadi::AgentBase::Observer
{
  Q_OBJECT
  
  public:
    ContactsResource (const QString &id);
    ~ContactsResource();
    
    using ResourceBase::synchronize;
    
  public Q_SLOTS:
    virtual void configure (WId windowID);
    
  protected Q_SLOTS:
    void retrieveCollections();
    void retrieveItems(const Akonadi::Collection& collection);
    bool retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts);

    void itemRemoved(const Akonadi::Item& item);
    void itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection);
    void itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers);
    
  protected:
    void aboutToQuit();
    
  private Q_SLOTS:
    void slotAbortRequested();
    void initialItemFetchJobFinished(KJob *job);
    void contactJobFinished(KJob *job);
    void contactListJobFinished(KJob *job);
    void photoJobFinished(KJob *job);
    
    void addJobFinished(KJob *job);
    void changeJobFinished(KJob *job);
    void removeJobFinished(KJob *job);

  private:
    void abort();
    void resetState();
    
    void updateLocalContacts();
    
    QMap<QString,KDateTime> m_existingContacts;
    QList<QString> m_pendingContacts;
    
    int m_contactsCnt;
    int m_photosCnt;
    
    bool m_idle;
    QList< QPointer<KJob> > m_currentJobs;
};

#endif // CONTACTSRESOURCE_H
