/*
    libKGoogle - Addressbook Service
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


#include "addressbook.h"
#include "objects/contact.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace KGoogle;
using namespace Service;

Addressbook::~Addressbook()
{ }

KGoogleObject* Addressbook::JSONToObject(const QByteArray& jsonData)
{
  Object::Contact *object = new Object::Contact();
  
  QJson::Parser parser;
  QVariantMap data = parser.parse(jsonData).toMap();
  
  
  /* Google contact ID. Store only the ID, not the entire URL */
  QString id = data["id"].toMap()["$t"].toString();
  object->setId(id.mid(id.lastIndexOf("/")+1));

  /* Google ETAG. This can be used to identify if the item was changed remotly */
  object->setEtag(data["gd$etag"].toString());
  
  object->setUpdated(KDateTime::fromString(data["updated"].toMap()["$t"].toString(), KDateTime::RFC3339Date));
  
  /* If the contact was deleted, we don't need more info about it.
   * Just store our own flag, which will be then parsed by the resource
   * itself. */
  object->setDeleted(data["gd$deleted"].isValid());
    
  /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture 
   * itself. */
  QVariantList links = data["link"].toList();
  foreach (const QVariant &link, links) {
    if (link.toMap()["type"] == "image/*") {
      object->setPhotoUrl(link.toMap()["href"].toString());
    }
  }
   
  /* Name */
  if (data.contains("title"))
    object->setName(data["title"].toMap()["$t"].toString());
   
  /* Note */
  if (data.contains("content"))
    object->setNotes(data["content"].toMap()["$t"].toString());

  /* Organization (work) - support only one organization */
  if (data.contains("gd$organization")) {
    QVariantMap organization = data["gd$organization"].toList().first().toMap();
    object->setJob(organization["gd$orgName"].toMap()["$t"].toString());
    object->setJobTitle(organization["gd$orgTitle"].toMap()["$t"].toString());
  }
    
  /* Emails */
  QVariantList emails = data["gd$email"].toList();
  foreach (const QVariant &em, emails) {
    QVariantMap email = em.toMap();
    object->addEmail(Object::Email(email["address"].toString(),
				   email["primary"].toBool()));
  }
    
  /* IMs */
  QVariantList ims = data["gd$im"].toList();
  foreach (const QVariant &i, ims) {
    QVariantMap im = i.toMap();
    object->addIM(Object::IM(im["address"].toString(),
			     im["protocol"].toString()));
  }

  /* Phone numbers */
  QVariantList phones = data["gd$phoneNumber"].toList();
  foreach (const QVariant &p, phones) {
    QVariantMap phone = p.toMap();
    object->addPhoneNumber(Object::PhoneNumber(phone["$t"].toString(),
					       phone["rel"].toString()));
  }
    
  /* Addresses */
  QVariantList addresses = data["gd$structuredPostalAddress"].toList();
  foreach (const QVariant &a, addresses) {
    QVariantMap address = a.toMap();
    object->addAddress(Object::Address(address,
				       address["rel"].toString()));
  }
    
  /* Birthday */
  QVariantMap bDay = data["gContact$birthday"].toMap();
  if (!bDay.isEmpty())
    object->setBirthday(bDay["when"].toString());
    
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */  
  
  return dynamic_cast< KGoogleObject* >(object);
}

QByteArray Addressbook::objectToJSON(KGoogleObject* object)
{
  Object::Contact *contact = static_cast<Object::Contact*>(object);
  
  QVariantMap output, data;
  QVariantMap item;
  /* Helper object for some parsing */
  KABC::Addressee addressee;
  
  /* ID */
  data["id"].toMap()["$t"] = "http://www.google.com/m8/feeds/contacts/default/base/" + contact->id();

  /* Name */
  QVariantMap name;
  addressee.setNameFromString(contact->name());
  name["gd$givenName"] = addressee.givenName();
  name["gd$familyName"] = addressee.familyName();
  name["gd$fullName"] = addressee.assembledName();
  data["gd$name"] = name;
  
  /* Notes */
  data["content"] = contact->notes();
  
  /* Emails */
  QVariantList emails;
  foreach (Object::Email email, contact->emails()) {
    QVariantMap em;
    /* FIXME: Is there a way to tell which type the email is? */
    em["rel"] = "http://schemas.google.com/g/2005#home";
    em["address"] = email.address();
    if (email.primary())
      em["primary"] = true;
    emails.append(em);
  }
  data["gd$email"] = emails;

  /* Phone numbers */
  QVariantList phones;
  foreach (Object::PhoneNumber number, contact->phoneNumbers()) {
    QVariantMap phone;
    phone["rel"] = Object::PhoneNumber::typeToScheme(number.type());
    phone["$t"] = number.number();
    phones.append(phone);
  }
  data["gd$phoneNumber"] = phones;

  /* Addresses */
  QVariantList addresses;
  foreach (Object::Address address, contact->addresses()) {
    QVariantMap addr;
    bool primary;
    addr["rel"] = Object::Address::typeToScheme(address.type(), &primary);
    addr["gd$city"] = address.locality();
    addr["gd$street"] = address.street();
    addr["gd$region"] = address.region();
    addr["gd$postcode"] = address.postalCode();
    addr["gd$country"] = address.country();
    addr["gd$formattedAddress"] = address.formattedAddress();
    if (primary)
      addr["primary"] = true;
    addresses.append(addr);
  }
  data["gd$structuredPostalAddress"] = addresses;
  
  if (!contact->birthday().isEmpty()) {
    QVariantMap bDay;
    bDay["when"] = contact->birthday();
    data["gContact$birthday"] = bDay;
  }
  
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */
  
  output["data"] = data;
  
  QJson::Serializer serializer;
  return serializer.serialize(output);
}

QList< KGoogleObject* > Addressbook::parseJSONFeed(const QByteArray& jsonFeed, FeedData* feedData)
{
  QList< KGoogleObject* > output;
  QJson::Parser parser;
  QJson::Serializer serializer;

  QVariantMap head = parser.parse(jsonFeed).toMap();
  
  QVariantMap feed = head["feed"].toMap();
  QVariantList entries = feed["entry"].toList();
  foreach (QVariant e, entries) {
    QByteArray entry = serializer.serialize(e);

    output.append(JSONToObject(entry));
  }
  
  if (feedData) {
    QVariantList links = feed["link"].toList();
    foreach (QVariant l, links) {
      QVariantMap link = l.toMap();
      if (link["rel"].toString() == "next") {
	feedData->nextLink = link["href"].toUrl();
	break;
      }
    }
    
    QVariantMap tmp = feed["openSearch$totalResults"].toMap();
    feedData->totalResults = tmp["$t"].toInt();
    
    tmp = feed["openSearch$startIndex"].toMap();
    feedData->startIndex = tmp["$t"].toInt();
    
    tmp = feed["openSearch$itemsPerPage"].toMap();
    feedData->itemsPerPage = tmp["$t"].toInt();
  }
  
  return output;
}


KGoogleObject* Addressbook::XMLToObject(const QByteArray& xmlData)
{
  Object::Contact *contact = new Object::Contact();
  
  QByteArray xmlDoc;
  /* Document without <xml> header is not valid and Qt won't parse it */
  if (!xmlData.contains("<?xml"))
    xmlDoc.append("<?xml version='1.0' encoding='UTF-8'?>");
  xmlDoc.append(xmlData);
  
  QDomDocument doc;
  doc.setContent(xmlDoc);
  QDomNodeList entry = doc.elementsByTagName("entry");
  QDomNodeList data;
  if (entry.length() > 0) {
    data = entry.at(0).childNodes();
  } else {
    return 0;
  }
  
  for (int i = 0; i < data.count(); i++) {
    QDomNode n = data.at(i);
    QDomElement e = n.toElement();
    
    /* Google contact ID. Store only the ID, not the entire URL */
    if (e.tagName() == "id") {
      QString id = e.text();
      contact->setId(id.mid(id.lastIndexOf("/")+1));
      continue;
    }
    
    /* ETag */
    if (e.tagName() == "etag") {
      contact->setEtag(e.text());
      continue;
    }
    
    /* If the contact was deleted, we don't need more info about it.
     * Just store our own flag, which will be then parsed by the resource
     * itself. */
    contact->setDeleted (e.tagName() == "gd:deleted");

    /* Store URL of the picture. The URL will be used later by PhotoJob to fetch the picture 
     * itself. */
    if (e.tagName() == "link" && e.attribute("rel") == "http://schemas.google.com/contacts/2008/rel#photo") {
      contact->setPhotoUrl(e.attribute("href"));	/* URL */
      continue;
    }
    
    /* Name */
    if (e.tagName() == "title") {
      contact->setName(e.text());
      continue;
    }
    
    /* Note */
    if (e.tagName() == "content") {
      contact->setNotes(e.text());
      continue;
    }
    
    /* Organization (work) */
    if (e.tagName() == "gd:organization") {
      QDomElement o = e.elementsByTagName("gd:orgName").at(0).toElement();
      if (!o.isNull())
	contact->setJob(o.text());
      continue;
    }
    
    /* Emails */
    if (e.tagName() == "gd:email") {
      contact->addEmail(Object::Email(e.attribute("address"),
				      (e.attribute("primary") == "true")));
      continue;
    }
    
    /* IMs */
    if (e.tagName() == "gd:im") {
      contact->addIM(Object::IM(e.attribute("address"),
			       e.attribute("protocol")));
      continue;
    }
    
    /* Phone numbers */
    if (e.tagName() == "gd:phoneNumber") {
      contact->addPhoneNumber(Object::PhoneNumber(e.text(),
						  e.attribute("rel")));
      continue;
    }
    
    /* Addresses */
    /* FIXME */
    if (e.tagName() == "gd:structuredPostalAddress") {
      contact->addAddress(Object::Address(e.toElement(),
					  e.attribute("rel"),
					  (e.attribute("primary") == "true")));
      continue;
    }
    
    /* Birthday */
    if (e.tagName() == "gContact:birthday") {
      contact->setBirthday(e.attribute("when"));
    }
    
    /* TODO: Expand supported items.
     * http://code.google.com/apis/gdata/docs/2.0/elements.html
     * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
     */
  }

  return dynamic_cast< KGoogleObject* >(contact);  
}

QByteArray Addressbook::objectToXML(KGoogleObject* object)
{
  Object::Contact *contact = static_cast<Object::Contact*>(object);
  
  QByteArray output;
  /* Helper object for parsing */
  KABC::Addressee addressee;
  
  /* Name */
  addressee.setNameFromString(contact->name());
  output.append("<gd:name>");
  if (!addressee.givenName().isEmpty())
    output.append("<gd:givenName>").append(addressee.givenName().toLatin1()).append("</gd:givenName>");
  if (!addressee.familyName().isEmpty())
    output.append("<gd:familyName>").append(addressee.familyName().toLatin1()).append("</gd:familyName>");
  if (!addressee.assembledName().isEmpty())
    output.append("<gd:fullName>").append(addressee.assembledName().toLatin1()).append("</gd:fullName>");
  output.append("</gd:name>");
  
  /* Notes */
  if (!contact->notes().isEmpty())
    output.append("<atom:content type='text'>").append(contact->notes().toLatin1()).append("</atom:content>");
  
  /* Emails */
  foreach (Object::Email email, contact->emails()) {
    output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(email.address().toLatin1()).append("' />");
  }
  
  /* Phone numbers */
  foreach (Object::PhoneNumber number, contact->phoneNumbers()) {
    output.append("<gd:phoneNumber rel='")
	  .append(Object::PhoneNumber::typeToScheme(number.type()).toLatin1())
	  .append("'>")
	  .append(number.number().toLatin1())
	  .append("</gd:phoneNumber>");
  }
  
  foreach (Object::Address address, contact->addresses()) {
    output.append("<gd:structuredPostalAddress rel='")
	  .append(Object::Address::typeToScheme(address.type()).toLatin1())
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
  
  /* Birthday */
  if (!contact->birthday().isEmpty())
    output.append("<gContact:birthday when='").append(contact->birthday().toLatin1()).append("'/>");
  
  /* TODO: Expand supported items.
   * http://code.google.com/apis/gdata/docs/2.0/elements.html
   * http://api.kde.org/4.x-api/kdepimlibs-apidocs/kabc/html/classKABC_1_1Addressee.html
   */
  
  return output;
}

QList< KGoogleObject* > Addressbook::parseXMLFeed(const QByteArray& xmlFeed, FeedData* feedData)
{
  Q_UNUSED (xmlFeed);
  Q_UNUSED (feedData);
  
  return QList< KGoogleObject* >();
}

QString Addressbook::scopeUrl()
{
  return "https://www.google.com/m8/feeds/"; 
}

QString Addressbook::fetchAllUrl()
{
  // %1 = user (default or user@gmail.com)
  return "https://www.google.com/m8/feeds/contacts/%1/full";
}

QString Addressbook::fetchUrl()
{
    // %1 = user (default or user@gmail.com)
    // %2 = contact ID
  return "https://www.google.com/m8/feeds/contacts/%1/full/%2"; 
}

QString Addressbook::createUrl()
{
  // %1 = user (default or user@gmail.com)
  return "https://www.google.com/m8/feeds/contacts/%1/full";
}

QString Addressbook::updateUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = contact ID
  return "https://www.google.com/m8/feeds/contacts/%1/full/%2"; 
}

QString Addressbook::removeUrl()
{
  // %1 = user (default or user@gmail.com)
  // %2 = contact ID
  return "https://www.google.com/m8/feeds/contacts/%1/full/%2"; 
}

const QString Addressbook::protocolVersion()
{
  return "3.0";
}

bool Addressbook::supportsJSONRead(QString* urlParam)
{
  if (urlParam)
    *urlParam = "json";
  
  return true;
}

bool Addressbook::supportsJSONWrite(QString* urlParam)
{
  Q_UNUSED (urlParam)
  
  return false;
}
