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

#include <qjson/parser.h>

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
  
  req.setUrl("https://www.google.com/m8/feeds/contacts/default/full/"+m_contactId+"?alt=json&oauth_token="+m_accessToken);
  req.setRawHeader("Authorization", "OAuth "+m_accessToken.toLatin1());
  req.setRawHeader("GData-Version", "3.0");  
  
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

  QJson::Parser parser;
  bool ok;
  
  QVariantMap data = parser.parse(reply->readAll(), &ok).toMap();
  if (!ok) {
     setError(1);
     setErrorText("Invalid feed format");
     emitResult();
     return;
  }
     
  m_contact = ContactJob::JSONToKABC(data["entry"].toMap());
  
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
      addr.setUid(e.text().mid(e.text().lastIndexOf("/")+1));
    }
    
    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    if (e.tagName() == "gd:deleted") {
      addr.insertCustom("google_contacts", "x-removed", "1");
      break;
    }
    
    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture 
     * itself. */
    if (e.tagName() == "link" && e.attribute("rel") == "http://schemas.google.com/contacts/2008/rel#photo") {
      KABC::Picture pic;
      pic.setType(e.attribute("type")); /* Mime type */
      pic.setUrl(e.attribute("href"));	/* URL */
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

QByteArray ContactJob::KABCToXmlEntry(KABC::Addressee addressee)
{
  QByteArray output;
  
  /* Name */
  output.append("<gd:name>");
  if (!addressee.givenName().isEmpty())
    output.append("<gd:givenName>").append(addressee.givenName().toLatin1()).append("</gd:givenName>");
  if (!addressee.familyName().isEmpty())
    output.append("<gd:familyName>").append(addressee.familyName().toLatin1()).append("</gd:familyName>");
  if (!addressee.assembledName().isEmpty())
    output.append("<gd:fullName>").append(addressee.assembledName().toLatin1()).append("</gd:fullName>");
  output.append("</gd:name>");
  
  /* Notes */
  if (!addressee.note().isEmpty())
    output.append("<atom:content type='text'>").append(addressee.note().toLatin1()).append("</atom:content>");
  
  /* Emails */
  if (addressee.emails().length()) {
    foreach (const QString &email, addressee.emails()) {
      output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(email.toLatin1()).append("' />");
    }
  }
  
  /* Phone numbers */
  if (addressee.phoneNumbers().length()) {
    foreach (const KABC::PhoneNumber &number, addressee.phoneNumbers()) {
      output.append("<gd:phoneNumber rel='http://schemas.google.com/g/2005#")
			.append(number.typeLabel().toLower().toLatin1())
			.append("'>")
		    .append(number.number().toLatin1())
		    .append("</gd:phoneNumber>");
    }
  }
  
  if (addressee.addresses().length()) {
    foreach (const KABC::Address &address, addressee.addresses()) {
      output.append("<gd:structuredPostalAddress rel='http://schemas.google.com/g/2005#")
	      .append(address.typeLabel().toLower().toLatin1())
	      .append("'>");
      
      if (!address.locality().isEmpty())
	output.append("<gd:city>").append(address.locality().toLatin1()).append("</gd:city>");
      if (!address.street().isEmpty())
	output.append("<gd:street>").append(address.street().toLatin1()).append("</gd:street>");
      if (!address.region().isEmpty())
	output.append("<gd:region>").append(address.region().toLatin1()).append("</gd:region>");
      if (!address.postalCode().isEmpty())
	output.append("<gd:postcode>").append(address.postalCode().toLatin1()).append("</gd:postcode>");
      if (!address.country().isEmpty())
	output.append("<gd:country>").append(address.country().toLatin1()).append("</gd:country>");
      if (!address.formattedAddress().isEmpty())
	output.append("<gd:formattedAddress>").append(address.formattedAddress().toLatin1()).append("</gd:formattedAddress>");
      
      output.append("</gd:structuredPostalAddress>");
    }
  }
  
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */
  
  return output;
}


KABC::Addressee ContactJob::JSONToKABC(QVariantMap entry)
{
  KABC::Addressee addr;
  
  /* Google contact ID. Store only the ID, not the entire URL */
  QString uid = entry["id"].toMap()["$t"].toString();
  addr.setUid( uid.mid(uid.lastIndexOf("/")+1) );
  
  /* Google ETAG. This can be used to identify if the item was changed remotly */
  addr.insertCustom("google_contacts", "x-etag", entry["gd$etag"].toString());
  
  /* If the contact was deleted, we don't need more info about it.
   * Just store our own flag, which will be then parsed by the resource
   * itself. */
  if (entry["gd:deleted"].isValid())
      addr.insertCustom("google_contacts", "x-removed", "1");
    
  /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture 
   * itself. */
  QVariantList links = entry["link"].toList();
  foreach (const QVariant &link, links) {
    if (link.toMap()["type"] == "image/*") {
      KABC::Picture pic;
      pic.setType(link.toMap()["type"].toString());
      pic.setUrl(link.toMap()["href"].toString());
      addr.setPhoto(pic);
    }
  }
   
  /* Name */
  addr.setNameFromString(entry["title"].toMap()["$t"].toString());
   
  /* Note */
  addr.setNote(entry["content"].toString());

  /* Organization (work) */
  QVariantMap organization = entry["gd$organization"].toMap();
  addr.setOrganization(organization["gd$orgName"].toString());
  addr.setTitle(organization["gd$title"].toString());
    
  /* Emails */
  QVariantList emails = entry["gd$email"].toList();
  foreach (const QVariant &em, emails) {
    QVariantMap email = em.toMap();
    addr.insertEmail(email["address"].toString(), 
		     email["primary"].toBool());
  }
    
  /* IMs */
  QVariantList ims = entry["gd$im"].toList();
  foreach (const QVariant &im, ims) {
    QString protocol = im.toMap()["protocol"].toString();
    protocol = protocol.mid(protocol.lastIndexOf("#")+1);
    addr.insertCustom("KADDRESSBOOK", "X-messaging/"+protocol, 
		      im.toMap()["address"].toString());
  }

  /* Phone numbers */
  QVariantList phones = entry["gd$phoneNumber"].toList();
  foreach (const QVariant &phone, phones) {
    QString type = phone.toMap()["rel"].toString();
    type = type.mid(type.lastIndexOf("#")+1);
    
    KABC::PhoneNumber phoneNumber;
    phoneNumber.setNumber(phone.toMap()["$t"].toString());
    
    if ((phone.toMap()["primary"].toBool()) ||
        (type == "main"))
      phoneNumber.setType(KABC::PhoneNumber::Pref);
    else if (type == "home")
      phoneNumber.setType(KABC::PhoneNumber::Home);
    else if (type == "work")
      phoneNumber.setType(KABC::PhoneNumber::Work);
    else if (type == "fax")
      phoneNumber.setType(KABC::PhoneNumber::Fax);
    else if (type == "mobile")
      phoneNumber.setType(KABC::PhoneNumber::Cell);
    else if (type == "car")
      phoneNumber.setType(KABC::PhoneNumber::Car);
    else if (type == "isdn")
      phoneNumber.setType(KABC::PhoneNumber::Isdn);
    else if (type == "pager")
      phoneNumber.setType(KABC::PhoneNumber::Pager);
      /* The following KABC values have no match in Google API:
	 Msg, Voice, Video, Bbs, Modem and Pcs. */

    addr.insertPhoneNumber(phoneNumber);    
  }
    
  /* Addresses */
  QVariantList addresses = entry["gd$postalAddress"].toList();
  foreach (const QVariant &a, addresses) {
    QVariantMap address = a.toMap();
    QString rel = address["rel"].toString();
    rel = rel.mid(rel.lastIndexOf("#")+1);
    
    KABC::Address adr;
    adr.setLabel(address["$t"].toString());
      
    if (rel == "work")
      adr.setType(KABC::Address::Work);
    else if (rel == "home")
      adr.setType(KABC::Address::Home);

    addr.insertAddress(adr);
  }
    
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */
  
  return addr;
}

QVariantMap ContactJob::KABCToJson(KABC::Addressee addressee)
{
  QVariantMap output;
  QVariantMap item;
  
  /* Name */
  QVariantMap name;
  name["gd$givenName"] = addressee.givenName();
  name["gd$familyName"] = addressee.familyName();
  name["gd$fullName"] = addressee.assembledName();
  output["gd$name"] = name;
  
  /* Notes */
  output["content"] = addressee.note();
  
  /* Emails */
  QVariantList emails;
  foreach (const QString &email, addressee.emails()) {
    QVariantMap em;
    /* FIXME: Is there a way to tell which type the email is? */
    em["rel"] = "http://schemas.google.com/g/2005#home";
    em["address"] = email;
    emails.append(em);
  }
  output["gd$email"] = emails;

  /* Phone numbers */
  QVariantList phones;
  foreach (const KABC::PhoneNumber &number, addressee.phoneNumbers()) {
    QVariantMap phone;
    phone["rel"] = "http://schemas.google.com/g/2005#" + number.typeLabel().toLower();
    phone["$t"] = number.number();
    phones.append(phone);
  }
  output["gd$phoneNumber"] = phones;

  /* Addresses */
  QVariantList addresses;
  foreach (const KABC::Address &address, addressee.addresses()) {
    QVariantMap addr;
    addr["rel"] = "http://schemas.google.com/g/2005#" + address.typeLabel().toLower();
    addr["gd$city"] = address.locality();
    addr["gd$street"] = address.street();
    addr["gd$region"] = address.region();
    addr["gd$postcode"] = address.postalCode();
    addr["gd$country"] = address.country();
    addr["gd$formattedAddress"] = address.formattedAddress();
    addresses.append(addr);
  }
  output["gd$structuredPostalAddress"] = addresses;
  
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */
  
  return output;
}