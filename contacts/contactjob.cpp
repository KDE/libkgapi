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

#include <KDE/KDebug>
#include <KDE/KABC/PhoneNumber>
#include <KDE/KABC/Address>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "contactjob.h"

ContactJob::ContactJob(const QString &contactId, const QString &accessToken):
  m_contactId(contactId),
  m_accessToken(accessToken)
{    
}

void ContactJob::start()
{
  QNetworkAccessManager *nam = new QNetworkAccessManager();
  QNetworkRequest req;
  
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  this, SLOT(contactRetrieved(QNetworkReply*)));
  connect(nam, SIGNAL(finished(QNetworkReply*)),
	  nam, SLOT(deleteLater()));
  
  req.setUrl("https://www.google.com/m8/feeds/contacts/default/full/"+m_contactId+"?oauth_token="+m_accessToken);
  req.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  
  nam->get(req);
}

void ContactJob::contactRetrieved(QNetworkReply *reply)
{
   if (reply->error() != QNetworkReply::NoError) {
     qDebug() << "Failed to fetch contact: " + reply->errorString();
     setError(1);
     setErrorText("Failed to fetch contact: " + reply->errorString());
     emitResult();
     return;
   }
   
  QDomDocument doc;
  doc.setContent(reply->readAll());
  
  QDomElement root = doc.documentElement();
  if (root.tagName() != "feed") {
     qDebug() << "Invalid feed format (root tag is " << root.tagName() << ")";
     setError(1);
     setErrorText("Invalid feed format");
     emitResult();
     return;
  }
     
  QDomNode n = root.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
   
      /* Contact entry */
      if (e.tagName() == "entry") {
	m_contact = ContactJob::xmlEntryToKABC(e); 
      }
    }
    n = n.nextSibling();    
  }
  
  setError(0);
  emitResult();
}

KABC::Addressee ContactJob::xmlEntryToKABC(QDomElement entry)
{
  KABC::Addressee addr;
  
  for (QDomNode n = entry.firstChild(); !n.isNull(); n = n.nextSibling()) {
    QDomElement e = n.toElement();
    
    /* Google contact ID. Store only the ID, not the entire URL */
    if (e.tagName() == "id") {
      addr.setUid(e.text().mid(e.text().lastIndexOf("/")+1);
    }
    
    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    if (e.tagName() == "gd:deleted") {
      addr.insertCustom("google_contacts", "x-removed", "1");
      break;
    }
    
    /* Name */
    if (e.tagName() == "title") {
      addr.setNameFromString(e.text());
      continue;
    }
    
    /* Note */
    if (e.tagName() == "content") {
      addr.setNote(e.text());
      continue;
    }
    
    /* Organization (work) */
    if (e.tagName() == "gd:organization") {
      QDomElement o = e.elementsByTagName("gd:orgName").at(0).toElement();
      if (!o.isNull())
	addr.setOrganization(o.text());
      continue;
    }
    
    /* Emails */
    if (e.tagName() == "gd:email") {
      addr.insertEmail(e.attribute("address"),
		       (e.attribute("primary")=="true"));
      continue;
    }
    
    /* IMs */
    if (e.tagName() == "gd:im") {
      QString protocol = e.attribute("protocol");
      protocol = protocol.mid(protocol.lastIndexOf("#")+1);
      
      addr.insertCustom("KADDRESSBOOK", "X-messaging/"+protocol, e.attribute("address"));
      continue;
    }
    
    /* Phone numbers */
    if (e.tagName() == "gd:phoneNumber") {
      QString rel = e.attribute("rel");
      rel = rel.mid(rel.lastIndexOf("#")+1);
      
      KABC::PhoneNumber phoneNumber;
      phoneNumber.setNumber(e.text());
      if (rel == "work")
	phoneNumber.setType(KABC::PhoneNumber::Work);
      else if (rel == "home")
	phoneNumber.setType(KABC::PhoneNumber::Home);
      else if (rel == "mobile")
	phoneNumber.setType(KABC::PhoneNumber::Cell);
      /* TODO: Extend list of supported number types:
       * http://code.google.com/apis/gdata/docs/2.0/elements.html#gdPhoneNumber
       * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1PhoneNumber.html
       */
      
      addr.insertPhoneNumber(phoneNumber);    
      continue;
    }
    
    /* Addresses */
    if (e.tagName() == "gd:postalAddress") {
      QString rel = e.attribute("rel");
      rel = rel.mid(rel.lastIndexOf("#")+1);
      
      KABC::Address address;
      /* TODO: Try to parse the address and store the parts properly
       * by setStreet(), setCountry(), setLocality(), setRegion(),
       * setPostOfficeBox() and setPostalCode()
       */
      address.setLabel(e.text());
      
      if (rel == "work")
	address.setType(KABC::Address::Work);
      else if (rel == "home")
	address.setType(KABC::Address::Home);
      
      addr.insertAddress(address);
      continue;
    }
    
    /* TODO: Expand supported items.
     * http://code.google.com/apis/gdata/docs/2.0/elements.html
     * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
     */
  }
  
  return addr;
}