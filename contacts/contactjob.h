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


#ifndef CONTACTJOB_H
#define CONTACTJOB_H

#include <KDE/KJob>
#include <KDE/KABC/Addressee>

#include <QtNetwork/QNetworkReply>

#include <QtXml/QDomElement>

/**
 * @brief A KDE Job for fetching individual contacts from
 * 	  Google Contacts using Google's Contacts Data API.
 * 
 * The Contact Data API developer guide is available on
 * http://code.google.com/apis/contacts/docs/3.0/developers_guide_protocol.html
 * 
 * See http://code.google.com/apis/gdata/docs/2.0/elements.html for full
 * list of properties, namespaces and elements that Google Data API supports.
 */
class ContactJob: public KJob
{
  Q_OBJECT
  public:
    /**
     * @brief Creates a new job for contact \contactId.
     * 
     * @param contactId Unique ID of contact on server. Google identified 
     * 			contacts by full URL and so do we.
     * @param accessToken Access token for Google's Contacts Data API
     */
    ContactJob(const QString &contactId, const QString &accessToken);
    
    /**
     * @brief Starts the job.
     */
    virtual void start();
    
    /**
     * @brief Returns fetched contact.
     * 
     * @return Fetched contact
     * */
    KABC::Addressee contact() const { return m_contact; }
    
    /**
     * @brief Converts the XML \entry to KABC::Addressee object.
     * 
     * @param entry A XML entry &lt;entry&gt; representing a contact as returned
     * 		    by Google API.
     * @return Returns intialized KABC::Addressee object.
     */
    static KABC::Addressee xmlEntryToKABC (QDomElement entry);
    
    /**
     * @brief Converts KABC::Addressee to Atom/XML format that can be
     * 	      send directly to Google.
     * @param addressee Adressee object to convert
     * @return XML representation of the \addressee. The XML code does not contain
     * 	       headers, only the contact data itself. The headers may vary accross requests
     * 	       (creating vs. modifying contact), therefor it's up to the specific job to prepend
     * 	       and append proper header and footer elements.
     */
    static QByteArray KABCToXmlEntry (KABC::Addressee addressee);
    
    /**
     * @brief Converts the JSON data (structured in QVariantMap) to KABC::Addressee
     * 
     * @param entry An JSON data structured in QVariantMap representing data from
     * 		    Google Calendar
     * @return Returns intialized KABC::Addressee object.
     */
    static KABC::Addressee JSONToKABC (QVariantMap entry);
    
    /**
     * @brief Converts KABC::Addressee to JSON format that can be
     * 	      send directly to Google.
     * @param addressee Adressee object to convert
     * @return JSON representation of the \addressee.
     */
    static QVariantMap KABCToJson (KABC::Addressee addressee);    
    
  private Q_SLOTS:
    /**
     * @private
     * @brief When asynchronous retrieving of data is finished (e.g. when
     * 	      reply from Google APi is received, this slot is invoked
     * 
     * @param reply 
     */
    void contactRetrieved(QNetworkReply *reply);
    
   
  private:
    QString m_contactId;
    QString m_accessToken;
    KABC::Addressee m_contact;
  
};

#endif // CONTACTJOB_H
