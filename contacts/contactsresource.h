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

/**
 * @brief An Akonadi resource for retrieving contacts from
 * 	  Google Contacts service.
 * 
 * The resource is using asynchronous KJobs for retrieving, inserting,
 * updating and removing contacts from remote address book.
 * 
 * As each Google account can have only one address book, one resource
 * for each Google accounts has to be created if user wants to have 
 * access to multiple addressbook accross multiple accounts.
 */
class ContactsResource: public Akonadi::ResourceBase,
			public Akonadi::AgentBase::Observer
{
  Q_OBJECT
  
  public:
    /**
     * @brief Creates new resource.
     * 
     * @param id Unique identificator of the resource assigned by Akonadi
     */
    ContactsResource (const QString &id);
    
    ~ContactsResource();
    
    using ResourceBase::synchronize;
    
  public Q_SLOTS:
    /**
     * @brief Opens configuration dialog
     * 
     * As of now, the AuthDialog is invoked without any
     * other settings dialog informing about what's going
     * on or allowing to revoke the tokens.
     * 
     * @todo Display a dialog with options to revoke tokens
     * 	     or authenticate and with an information about
     * 	     account to which the resource is authenticated
     */
    virtual void configure (WId windowID);
    
    
  protected Q_SLOTS:
    /**
     * @brief Defined list of collections. 
     */
    void retrieveCollections();
    
    /**
     * @brief Retrieves all items from remote server.
     * 
     * @todo Don't fetch all items every time from remote server,
     * 	     ask only for items updated since last synchronization.
     */
    void retrieveItems(const Akonadi::Collection& collection);
    
    /**
     * @brief Retrieves single contactJobFinished
     */
    bool retrieveItem(const Akonadi::Item& item, const QSet< QByteArray >& parts);

    void itemRemoved(const Akonadi::Item& item);
    void itemAdded(const Akonadi::Item& item, const Akonadi::Collection& collection);
    void itemChanged(const Akonadi::Item& item, const QSet< QByteArray >& partIdentifiers);
    
    void emitPercent(KJob *job, ulong progress) { emit percent(progress); Q_UNUSED (job) }
    
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
