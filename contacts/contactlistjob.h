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


#ifndef CONTACTLISTJOB_H
#define CONTACTLISTJOB_H

#include <KDE/KABC/Addressee>
#include <KDE/KJob>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "contact.h"

/**
 * @brief A KDE Job class for fetching list of contacts using
 * 	  Google's Contacts Data API.
 *
 * The Contact Data API developer guide is available on
 * http://code.google.com/apis/contacts/docs/3.0/developers_guide_protocol.html
 * 
 * See http://code.google.com/apis/gdata/docs/2.0/elements.html for full
 * list of properties, namespaces and elements that Google Data API supports.
 */
class ContactListJob: public KJob
{
  Q_OBJECT
  public:
    /**
     * @brief Creates a new job that will fetch all contacts from users address book.
     * 
     * @param accessToken Access token for Google's Contacts Data API
     * @param lastSync Timestamp of last synchronization. We will ask only for items that
     * 		       were changed or removed since the \lastSync.
     * 		       When empty, all items will be fetched.
     */
    ContactListJob(const QString &accessToken,
		   const QString &lastSync = QString());

    virtual ~ContactListJob();
    
    /**
     * @brief Starts the job.
     */
    virtual void start();
    
    /**
     * @brief Returns list of ABC::Addressee objects (see @see{ContactJob::xmlEntryToKABC}).
     * 
     * When 
     * 
     * @return Returns list of KABC::Addressee objects, for each contact 
     */
    QList<Contact::Contact*>* contacts() { return m_contacts; }
    
  private Q_SLOTS:
    void contactListRetrieved(QNetworkReply *reply);
    
  private:
    void requestContacts(const QUrl &url);
    
    QNetworkAccessManager *m_nam;
    QString m_accessToken;
    QString m_lastSync;
    
    QList<Contact::Contact*> *m_contacts;
    
};

#endif // CONTACTLISTJOB_H
