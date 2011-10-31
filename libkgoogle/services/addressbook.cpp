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
#include "kgoogleaccessmanager.h"

#include <kurl.h>

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
  
  /* Date and time when contact was updated on the remote server */
  object->setUpdated(KGoogleAccessManager::RFC3339StringToDate(data["updated"].toMap()["$t"].toString()));
  
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
  
  /* Formatted name */
  if (data.contains("gd$name")) {
    QVariantMap name = data["gd$name"].toMap();
    
    if (name.contains("gd$fullName"))
      object->setName(name["gd$fullName"].toMap()["$t"].toString());
    if (name.contains("gd$givenName"))
      object->setGivenName(name["gd$givenName"].toMap()["$t"].toString());
    if (name.contains("gd$familyName"))
      object->setFamilyName(name["gd$familyName"].toMap()["$t"].toString());
    if (name.contains("gd$additionalName"))
      object->setAdditionalName(name["gd$additionalName"].toMap()["$t"].toString());;
    if (name.contains("gd$namePrefix"))
      object->setPrefix(name["gd$namePrefix"].toMap()["$t"].toString());
    if (name.contains("gd$nameSuffix"))
      object->setSuffix(name["gd$nameSuffix"].toMap()["$t"].toString());
  }
       
  /* Note */
  if (data.contains("content"))
    object->setNote(data["content"].toMap()["$t"].toString());

  /* Organization (work) - KABC supports only one organization */
  if (data.contains("gd$organization")) {
    QVariantMap organization = data["gd$organization"].toList().first().toMap();
    
    if (organization.contains("gd$orgName"))
      object->setOrganization(organization["gd$orgName"].toMap()["$t"].toString());

    if (organization.contains("gd$orgDepartment"))
      object->setDepartment(organization["gd$orgDepartment"].toMap()["$t"].toString());
    
    if (organization.contains("gd$orgTitle"))
      object->setTitle(organization["gd$orgTitle"].toMap()["$t"].toString());
    
    if (organization.contains("gd$where"))
      object->insertCustom("X-KADDRESSBOOK", "X-Office", organization["gd$where"].toMap()["$t"].toString());
  }
  
  /* Nickname */
  if (data.contains("gContact$nickname")) {
    object->setNickName(data["gContact$nickname"].toMap()["$t"].toString());
  }
  
  /* Occupation (= organization/title) */
  if (data.contains("gContact$occupation")) {
    object->insertCustom("KADDRESSBOOK", "X-Profession", data["gContact$occupation"].toMap()["$t"].toString());
  }
  
  /* Relationships */
  if (data.contains("gContact$relation")) {
    foreach (QVariant r, data["gContact$relation"].toList()) {
      QVariantMap relation = r.toMap();
      if (relation["rel"].toString() == "spouse") {
	object->insertCustom("KADDRESSBOOK", "X-SpousesName", relation["$t"].toString());
	continue;
      }
      
      if (relation["rel"].toString() == "manager") {
	object->insertCustom("KADDRESSBOOK", "X-ManagersName", relation["$t"].toString());
	continue;
      }
      
      if (relation["rel"].toString() == "assistant") {
	object->insertCustom("KADDRESSBOOK", "X-AssistantsName", relation["$t"].toString());
	continue;
      }
    }
  }
  
  /* Anniversary */
  if (data.contains("gContact$event")) {
    foreach (QVariant e, data["gContact$event"].toList()) {
      QVariantMap event = e.toMap();

      if (event["rel"].toString() == "anniversary") {
	QVariantMap when = event["gd$when"].toMap();
	object->insertCustom("KADDRESSBOOK", "X-Anniversary", when["startTime"].toString());
      }
    }
  }
  
  /* Websites */
  if (data.contains("gContact$website")) {
    foreach (QVariant w, data["gContact$website"].toList()) {
      QVariantMap web = w.toMap();
      
      if (web["rel"].toString() == "home-page") {
	object->setUrl(KUrl(web["href"].toString()));
	continue;
      }
      
      if (web["rel"].toString() == "blog") {
	object->insertCustom("KADDRESSBOOK", "BlogFeed", web["href"].toString());
	continue;
      }
    }
  }
  
  /* Emails */
  QVariantList emails = data["gd$email"].toList();
  foreach (const QVariant &em, emails) {
    QVariantMap email = em.toMap();
    object->insertEmail(email["address"].toString(), email["primary"].toBool());
  }
    
  /* IMs */
  QVariantList ims = data["gd$im"].toList();
  foreach (const QVariant &i, ims) {
    QVariantMap im = i.toMap();
    object->insertCustom("messaging/" + Object::Contact::IMSchemeToProtocolName(im["protocol"].toString()), "All", im["address"].toString());
  }

  /* Phone numbers */
  QVariantList phones = data["gd$phoneNumber"].toList();
  foreach (const QVariant &p, phones) {
    QVariantMap phone = p.toMap();
    object->insertPhoneNumber(KABC::PhoneNumber(phone["$t"].toString(),
						Object::Contact::phoneSchemeToType(phone["rel"].toString())));
  }
    
  /* Addresses */
  QVariantList addresses = data["gd$structuredPostalAddress"].toList();
  foreach (const QVariant &a, addresses) {
    QVariantMap address = a.toMap();
    KABC::Address addr;
    if (!address.contains("gd$city") && 
      !address.contains("gd$country") &&
      !address.contains("gd$postcode") &&
      !address.contains("gd$region") &&
      !address.contains("gd$pobox")) {
      addr.setExtended(address["gd$street"].toMap()["$t"].toString());
    } else {
      if (address.contains("gd$street"))
	addr.setStreet(address["gd$street"].toMap()["$t"].toString());
      if (address.contains("gd$country"))
	addr.setCountry(address["gd$country"].toMap()["$t"].toString());
      if (address.contains("gd$city"))
	addr.setLocality(address["gd$city"].toMap()["$t"].toString());
      if (address.contains("gd$postcode"))
	addr.setPostalCode(address["gd$postcode"].toMap()["$t"].toString());
      if (address.contains("gdregion"))
	addr.setRegion(address["gd$region"].toMap()["$t"].toString());
      if (address.contains("gd$pobox"))
	addr.setPostOfficeBox(address["gd$pobox"].toMap()["$t"].toString());
    }
    addr.setType(Object::Contact::addressSchemeToType(address["rel"].toString()));
    
    object->insertAddress(addr);
  }
    
  /* Birthday */
  QVariantMap bDay = data["gContact$birthday"].toMap();
  if (!bDay.isEmpty())
    object->setBirthday(QDateTime::fromString(bDay["when"].toString(), "%Y-%m-%d"));
    
  /* User-defined fields */
  QVariantList userDefined = data["gContact$userDefinedField"].toList();
  foreach (const QVariant &u, userDefined) {
    QVariantMap field = u.toMap();
    object->insertCustom("GCALENDAR", field["key"].toString(), field["value"].toString());
  }

  /* Groups */
  QVariantList groups = data["gContact$groupMembershipInfo"].toList();
  QStringList groupsList;
  foreach (const QVariant &g, groups) {
    QVariantMap group = g.toMap();
    if (group["deleted"].toBool() == false)
	groupsList.append(group["href"].toString());
  }
  object->insertCustom("GCALENDAR", "groupMembershipInfo", groupsList.join(","));

  return dynamic_cast< KGoogleObject* >(object);
}

QByteArray Addressbook::objectToJSON(KGoogleObject* object)
{
  /* Google allows writing only XML data */
  return QByteArray();
  
  Q_UNUSED(object)
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

  QStringList groups;

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
      contact->setNote(e.text());
      continue;
    }
    
    /* Organization (work) - KABC supports only organization */
    if (e.tagName() == "gd:organization") {
      QDomNodeList l = e.childNodes();
      for (uint i = 0; i < l.length(); i++) {
	QDomElement el = l.at(i).toElement();
	
	if (el.tagName() == "gd:orgName") {
	  contact->setOrganization(el.text());
	  continue;
	}
	
	if (el.tagName() == "gd:orgDepartment") {
	  contact->setDepartment(el.text());
	  continue;
	}
	
	if (el.tagName() == "gd:orgTitle") {
	  contact->setTitle(el.text());
	  continue;
	}
	
	if (el.tagName() == "gd:where") {
	  contact->insertCustom("KADDRESSBOOK", "X-Office", el.text());
	  continue;
	}
      }
      continue;
    }
    
    /* Nickname */
    if (e.tagName() == "gContact:nickname") {
      contact->setNickName(e.text());
      continue;
    }
    
    /* Occupation (= organization/title) */
    if (e.tagName() == "gContact:occupation") {
      contact->insertCustom("KADDRESSBOOK", "X-Profession", e.text());
      continue;
    }
    
    /* Relationships */
    if (e.tagName() == "gContact:relation") {
      if (e.attribute("rel", "") == "spouse") {
	contact->insertCustom("KADDRESSBOOK", "X-SpousesName", e.text());
	continue;
      }
      
      if (e.attribute("rel", "") == "manager") {
	contact->insertCustom("KADDRESSBOOK", "X-ManagersName", e.text());
	continue;
      }
      
      if (e.attribute("rel", "") == "assistant") {
	contact->insertCustom("KADDRESSBOOK", "X-AssistantsName", e.text());
	continue;
      }
      
      continue;
    }
    
    /* Anniversary */
    if (e.tagName() == "gContact:event") {
      if (e.attribute("rel", "") == "anniversary") {
	QDomElement w = e.firstChildElement("gd:when");
	contact->insertCustom("KADDRESSBOOK", "X-Anniversary", w.attribute("startTime", ""));
      }

      continue; 
    }
    
    /* Websites */
    if (e.tagName() == "gContact:website") {
      if (e.attribute("rel", "") == "home-page") {
	contact->setUrl(KUrl(e.attribute("href", "")));
	continue;
      }
      
      if (e.attribute("rel", "") == "blog") {
	contact->insertCustom("KADDRESSBOOK", "BlogFeed", e.attribute("href", ""));
	continue;
      }

      continue;
    }
    
    /* Emails */
    if (e.tagName() == "gd:email") {
      contact->insertEmail(e.attribute("address"), (e.attribute("primary").toLower() == "true"));
      continue;
    }
    
    /* IMs */
    if (e.tagName() == "gd:im") {
      contact->insertCustom("messaging/" + Object::Contact::IMSchemeToProtocolName(e.attribute("protocol")), "All", e.attribute("address"));
      continue;
    }
    
    /* Phone numbers */
    if (e.tagName() == "gd:phoneNumber") {
      contact->insertPhoneNumber(KABC::PhoneNumber(e.text(), Object::Contact::phoneSchemeToType(e.attribute("rel"))));
      continue;
    }
    
    /* Addresses */
    /* FIXME */
    if (e.tagName() == "gd:structuredPostalAddress") {
      KABC::Address address;
      QDomNodeList l = e.childNodes();
      for (uint i = 0; i < l.length(); i++) {
	QDomElement el = l.at(i).toElement();
        
	if (el.tagName() == "gd:street") {
	  address.setStreet(el.text());
	  continue;
	}
	  
	if (el.tagName() == "gd:country") {
	  address.setCountry(el.text());
	  continue;
	}
	  
	if (el.tagName() == "gd:city") {
	  address.setLocality(el.text());
	  continue;
	}
	  
	if (el.tagName() == "gd:postcode") {
	  address.setPostalCode(el.text());
	  continue;
	}
	  
	if (el.tagName() == "gd:region") {
	  address.setRegion(el.text());
	  continue;
	}
	  
	if (el.tagName() == "gd:pobox") {
	  address.setPostOfficeBox(el.text());
	  continue;
	}
      }

      address.setType(Object::Contact::addressSchemeToType(e.attribute("rel"), (e.attribute("primary") == "true")));
      contact->insertAddress(address);
      continue;
    }

    /* Birthday */
    if (e.tagName() == "gContact:birthday") {
      contact->setBirthday(QDateTime::fromString(e.attribute("when"), "%Y-%m-%d"));
      continue;
    }

    /* User-defined tags */
    if (e.tagName() == "gContact:userDefinedField") {
      contact->insertCustom("GCALENDAR", e.attribute("key", ""), e.attribute("value", ""));
      continue;
    }

    if (e.tagName() == "gContact:groupMembershipInfo") {
      if (e.hasAttribute("deleted") || e.attribute("deleted").toInt() == false) {
	  groups.append(e.attribute("href"));
      }
    }
  }

  contact->insertCustom("GCALENDAR", "groupMembershipInfo", groups.join(","));

  return dynamic_cast< KGoogleObject* >(contact);
}

QByteArray Addressbook::objectToXML(KGoogleObject* object)
{
  Object::Contact *contact = static_cast<Object::Contact*>(object);
  
  QByteArray output;
  
  QStringList parsedCustoms;
  
  /* Name */
  output.append("<gd:name>");
  if (!contact->givenName().isEmpty())
    output.append("<gd:givenName>").append(contact->givenName().toLatin1()).append("</gd:givenName>");
  if (!contact->familyName().isEmpty())
    output.append("<gd:familyName>").append(contact->familyName().toLatin1()).append("</gd:familyName>");
  if (!contact->assembledName().isEmpty())
    output.append("<gd:fullName>").append(contact->assembledName().toLatin1()).append("</gd:fullName>");
  if (!contact->additionalName().isEmpty())
    output.append("<gd:additionalName>").append(contact->additionalName().toLatin1()).append("</gd:additionalName>");
  if (!contact->prefix().isEmpty())
    output.append("<gd:namePrefix>").append(contact->prefix().toLatin1()).append("</gd:namePrefix>");
  if (!contact->suffix().isEmpty())
    output.append("<gd:nameSuffix>").append(contact->suffix().toLatin1()).append("</gd:nameSuffix>");
  output.append("</gd:name>");
  
  /* Notes */
  if (!contact->note().isEmpty())
    output.append("<atom:content type='text'>").append(contact->note().toLatin1()).append("</atom:content>");
  
  /* Organization (work) */
  QByteArray org;
  if (!contact->organization().isEmpty())
    org.append("<gd:orgName>").append(contact->organization().toLatin1()).append("</gd:orgName>");
  if (!contact->department().isEmpty())
    org.append("<gd:orgDepartment>").append(contact->department().toLatin1()).append("</gd:orgDepartment>");
  if (!contact->title().isEmpty())
    org.append("<gd:orgTitle>").append(contact->title().toLatin1()).append("</gd:orgTitle>");
  if (!contact->custom("KADDRESSBOOK", "X-Office").isEmpty()) {
    org.append("<gd:where>").append(contact->custom("KADDRESSBOOK", "X-Office").toLatin1()).append("</gd:where>");
    parsedCustoms << "KADDRESSBOOK-X-Office";
  }
  if (!org.isEmpty())
    output.append("<gd:organization rel=\"http://schemas.google.com/g/2005#work\">").append(org).append("</gd:organization>");

  /* Nickname */
  if (!contact->nickName().isEmpty())
    output.append("<gContact:nickname>").append(contact->nickName().toLatin1()).append("</gContact:nickname>");
  
  /* Occupation */
  if (!contact->custom("KADDRESSBOOK", "X-Profession").isEmpty()) {
    output.append("<gContact:occupation>").append(contact->custom("KADDRESSBOOK", "X-Profession").toLatin1()).append("</gContact:occupation>");
    parsedCustoms << "KADDRESSBOOK-X-Profession";
  }
  
  /* Spouse */
  QString spouse = contact->custom("KADDRESSBOOK", "X-SpousesName");
  if (!spouse.isEmpty()) {
    output.append("<gContact:relation rel=\"spouse\">").append(spouse.toLatin1()).append("</gContact:relation>");
    parsedCustoms << "KADDRESSBOOK-X-SpousesName";
  }
  
  /* Manager */
  QString manager = contact->custom("KADDRESSBOOK", "X-ManagersName");
  if (!manager.isEmpty()) {
    output.append("<gContact:relation rel=\"manager\">").append(manager.toLatin1()).append("</gContact:relation>");
    parsedCustoms << "KADDRESSBOOK-X-ManagersName";
  }
  
  /* Assistant */
  QString assistant = contact->custom("KADDRESSBOOK", "X-AssistantsName");
  if (!assistant.isEmpty()) {
    output.append("<gContact:relation rel=\"assistant\">").append(assistant.toLatin1()).append("</gContact:relation>");
    parsedCustoms << "KADDRESSBOOK-X-AssistantsName";
  }
  
  /* Anniversary */
  QString anniversary = contact->custom("KADDRESSBOOK", "X-Anniversary");
  if (!anniversary.isEmpty()) {
    output.append("<gContact:event rel=\"anniversary\"><gd:when startTime=\"").append(anniversary.toLatin1()).append("\" /></gContact:event>");
    parsedCustoms << "KADDRESSBOOK-X-Anniversary";
  }
  
  /* Homepage */
  if (!contact->url().isEmpty())
    output.append("<gContact:website rel=\"home-page\" href=\"").append(contact->url().prettyUrl().toLatin1()).append("\" />");
  
  /* Blog */
  QString blog = contact->custom("KADDRESSBOOK", "BlogFeed");
  if (!blog.isEmpty()) {
    output.append("<gContact:website rel=\"blog\" href=\"").append(blog.toLatin1()).append("\" />");
    parsedCustoms << "KADDRESSBOOK-BlogFeed";
  }

  /* Emails */
  foreach (QString email, contact->emails()) {
    output.append("<gd:email rel='http://schemas.google.com/g/2005#home' address='").append(email.toLatin1()).append("' />");
  }
  
  /* IMs */
  QString im_str("<gd:im address=\"%1\" protocol=\"%2\" rel=\"http://schemas.google.com/g/2005#other\" primary=\"%3\"/>");
  foreach (QString im, contact->customs()) {
    if (im.startsWith("messaging/")) {
      QString key = im.left(im.indexOf(':'));
      QString value = im.mid(im.indexOf(':') + 1);
      QString proto = key.mid(10);
      proto.chop(4);
      bool primary = (contact->custom("KADDRESSBOOK", "X-IMAddress") == value);
      output.append(im_str.arg(value, Object::Contact::IMProtocolNameToScheme(proto), (primary ? "true" : "false")).toLatin1());
      parsedCustoms << key;
    }
  }
  parsedCustoms << "KADDRESSBOOK-X-IMAddress";
  
  /* Phone numbers */
  QString phone_str("<gd:phoneNumber rel=\"%1\">%2</gd:phoneNumber>");
  foreach (KABC::PhoneNumber number, contact->phoneNumbers()) {
    output.append(phone_str.arg(Object::Contact::phoneTypeToScheme(number.type()), number.number()).toLatin1());
  }
  
  /* Address */
  foreach (KABC::Address address, contact->addresses()) {
    output.append("<gd:structuredPostalAddress rel='")
	  .append(Object::Contact::addressTypeToScheme(address.type()).toLatin1())
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
  if (!contact->birthday().isNull()) {
    QString birthday = contact->birthday().toString("%Y-%m-%d");
    output.append("<gContact:birthday when='").append(birthday.toLatin1()).append("'/>");
  }

  QStringList groups = contact->custom("GCALENDAR", "groupMembershipInfo").split(",");
  if ((groups.length() > 0) && !groups.at(0).isEmpty()) {
    foreach (const QString &group, groups) {
      output.append(QString("<gContact:groupMembershipInfo deleted=\"false\" href=\"%1\" />").arg(group).toLatin1());
    }
  }
  parsedCustoms << "GCALENDAR-groupMembershipInfo";

  /* User-defined fields */
  QStringList customs = contact->customs();
  QString defined_str("<gContact:userDefinedField key=\"%1\" value=\"%2\" />");
  foreach (QString customStr, customs) {
    QString key = customStr.left(customStr.indexOf(':'));
    if (!parsedCustoms.contains(key)) {
      QString value = customStr.mid(customStr.indexOf(':') + 1);
      output.append(defined_str.arg(key, value).toLatin1());
    }
  }
    
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
