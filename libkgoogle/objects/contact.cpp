/*
    libKGoogle - KGoogleObject - Contact
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

#include "kgoogleobject.h"
#include "contact.h"
#include "contact_p.h"
#include "services/addressbook.h"

#include <qstring.h>
#include <qurl.h>
#include <qvariant.h>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <kabc/address.h>
#include <kabc/phonenumber.h>

using namespace KGoogle::Object;

#define SCHEME_URL "http://schemas.google.com/g/2005#"

/*************************** PRIVATE DATA ******************************/

ContactData::ContactData(const ContactData &other):
      QSharedData(other),
      deleted(other.deleted),
      photoUrl(other.photoUrl),
      name(other.name),
      notes(other.notes),
      job(other.job),
      jobTitle(other.jobTitle),
      emails(other.emails),
      ims(other.ims),
      phoneNumbers(other.phoneNumbers),
      addresses(other.addresses)
{ }

/***************************** CONTACT ************************************/

Contact::Contact()
{
  d = new ContactData;
}

Contact::Contact(const Contact &other):
  KGoogleObject(other),
  d(other.d)
{ }

Contact::~Contact()
{ }

void Contact::setDeleted(const bool deleted)
{
  d->deleted = deleted;
}

bool Contact::deleted()
{
  return d->deleted;
}

void Contact::setPhotoUrl(const QString& photoUrl)
{
  d->photoUrl = QUrl(photoUrl);
}

void Contact::setPhotoUrl(const QUrl& photoUrl)
{
  d->photoUrl = photoUrl;
}

QUrl Contact::photoUrl()
{
  return d->photoUrl;
}

void Contact::setName(const QString& name)
{
  d->name = name;
}

QString Contact::name()
{
  return d->name;
}

void Contact::setNotes(const QString& notes)
{
  d->notes = notes;
}

QString Contact::notes()
{
  return d->notes;
}

void Contact::setJob(const QString& job)
{
  d->job = job;
}

QString Contact::job()
{
  return d->job;
}

void Contact::setJobTitle(const QString &jobTitle)
{
  d->jobTitle = jobTitle;
}

QString Contact::jobTitle()
{
  return d->jobTitle;
}

void Contact::setBirthday(const QString& birthday)
{
  d->birthday = birthday;
}

void Contact::setCreated(const KDateTime& created)
{
  d->created = created;
}

KDateTime Contact::created()
{
  return d->created;
}

void Contact::setUpdated(const KDateTime& updated)
{
  d->updated = updated;
}

KDateTime Contact::updated()
{
  return d->updated;
}

QString Contact::birthday()
{
  return d->birthday;
}

void Contact::addEmail(const Email& email)
{
  d->emails << email;
}

Email::List Contact::emails()
{
  return d->emails;
}

void Contact::addIM(const IM& im)
{
  d->ims << im;
}

IM::List Contact::IMs()
{
  return d->ims;
}

void Contact::addPhoneNumber(const PhoneNumber &phoneNumber)
{
  d->phoneNumbers << phoneNumber;
}

void Contact::addPhoneNumber(const KABC::PhoneNumber &phoneNumber)
{
  addPhoneNumber(PhoneNumber(phoneNumber));
}

PhoneNumber::List Contact::phoneNumbers()
{
  return d->phoneNumbers;
}

void Contact::addAddress(const Address &address)
{
  d->addresses << address;
}

void Contact::addAddress(const KABC::Address& address)
{
  addAddress(Address(address));
}

Address::List Contact::addresses()
{
  return d->addresses;
}

void Contact::fromKABC(const KABC::Addressee* addressee)
{
  setId(addressee->uid());
  setEtag(addressee->custom("KADDRESSBOOK", "e-tag"));
  setDeleted(addressee->custom("KADDRESSBOOK", "deleted") == "true");
  setPhotoUrl(addressee->photo().url());
  
  setName(addressee->assembledName());
  setNotes(addressee->note());
  setJob(addressee->organization());
  setJobTitle(addressee->title());
  setBirthday(addressee->birthday().toString("yyyy-MM-dd"));
  foreach (QString email, addressee->emails())
    addEmail(Email(email));
  /* TODO: Fetch IM */
  foreach (KABC::PhoneNumber number, addressee->phoneNumbers())
    addPhoneNumber(number);
  foreach (KABC::Address address, addressee->addresses())
    addAddress(address);
}

KABC::Addressee* Contact::toKABC()
{
  KABC::Addressee *addressee = new KABC::Addressee();
  
  addressee->setUid(id());
  if (deleted())
    addressee->insertCustom("KADDRESSBOOK", "deleted", QString(deleted()));
  addressee->photo().setUrl(photoUrl().toString());
  
  addressee->setNameFromString(name());
  addressee->setNote(notes());
  addressee->setOrganization(job());
  addressee->setTitle(jobTitle());
  addressee->setBirthday(QDateTime::fromString(birthday(), "yyyy-MM-dd"));
  foreach (Email email, emails())
    addressee->insertEmail(email.address(), email.primary());
  /* TODO: Set IM */
  foreach (const PhoneNumber &number, phoneNumbers())
    addressee->insertPhoneNumber(number);
  foreach (const Address &address, addresses())
    addressee->insertAddress(address);
  
  
  
  return addressee;
}





/********************************** EMAIL ****************************/

Email::Email(const QString address, const bool primary):
  m_address(address),
  m_primary(primary)
{ }

bool Email::operator==(const Email& other)
{
  return ((m_primary == other.m_primary) &&
	  (m_address == other.m_address));
}


void Email::setAddress(const QString& address)
{
  m_address = address;
}

QString Email::address()
{
  return m_address;
}

void Email::setPrimary(const bool primary)
{
  m_primary = primary;
}

bool Email::primary()
{
  return m_primary;
}




/******************************* IM *******************************/

IM::IM(const QString& address, const QString& scheme):
  m_address(address),
  m_scheme(scheme)
{
  m_protocol = schemeToProtocol(scheme);
}

IM::IM(const QString& address, const IM::IMProtocol protocol):
  m_address(address),
  m_protocol(protocol)
{
  m_scheme = protocolToScheme(protocol);
}

bool IM::operator==(const IM& other)
{
  return ((m_address == other.m_address) &&
	  (m_protocol == other.m_protocol) &&
	  (m_scheme == other.m_scheme));
}

void IM::setAddress(const QString& address)
{
  m_address = address;
}

QString IM::address()
{
  return m_address;
}

void IM::setScheme(const QString& scheme)
{
  m_scheme = scheme;
  m_protocol = schemeToProtocol(scheme);
}

QString IM::scheme()
{
  return m_scheme;
}

void IM::setProtocol(const IM::IMProtocol protocol)
{
  m_protocol = protocol;
  m_scheme = protocolToScheme(protocol);
}

IM::IMProtocol IM::protocol()
{
  return m_protocol;
}

QString IM::protocolToScheme(const IM::IMProtocol protocol)
{
  QString protoName;
  
  switch (protocol) {
    case Jabber:
      protoName = "JABBER";
      break;
    case ICQ:
      protoName = "ICQ";
      break;
    case GoogleTalk:
      protoName = "GOOGLE_TALK";
      break;
    case QQ:
      protoName = "QQ";
      break;
    case Skype:
      protoName = "SKYPE";
      break;
    case Yahoo:
      protoName = "YAHOO";
      break;
    case MSN:
      protoName = "MSN";
      break;
    case AIM:
      protoName = "AIM";
      break;
    default:
      protoName = "OTHER";
  }

  return SCHEME_URL + protoName;
}

IM::IMProtocol IM::schemeToProtocol(const QString& scheme)
{
  QString protoName = scheme.mid(scheme.lastIndexOf("#")+1);

  if (protoName == "JABBER")
    return IM::Jabber;
  if (protoName == "ICQ")
    return IM::ICQ;
  if (protoName == "GOOGLE_TALK")
    return IM::GoogleTalk;
  if (protoName == "QQ")
    return IM::QQ;
  if (protoName == "SKYPE")
    return IM::Skype;
  if (protoName == "YAHOO")
    return IM::Yahoo;
  if (protoName == "MSN")
    return IM::MSN;
  if (protoName == "AIM")
    return IM::AIM;
  
  return IM::Other;
}




/********************************* ADDRESS **************************************/

Address::Address(const QString& street, const QString& pobox, const QString& locality, 
			  const QString& region, const QString& postalCode, const QString& country,
			  const KABC::Address::Type type)
{
  setStreet(street);
  setPostOfficeBox(pobox);
  setLocality(locality);
  setRegion(region);
  setPostalCode(postalCode);
  setCountry(country);
  setType(type);
}


Address::Address(const QVariantMap& address, const QString& scheme, const bool primary)
{
  if (!address.contains("gd$city") && 
      !address.contains("gd$country") &&
      !address.contains("gd$postcode") &&
      !address.contains("gd$region") &&
      !address.contains("gd$pobox")) {
    setExtended(address["gd$street"].toMap()["$t"].toString());
  } else {
    setStreet(address["gd$street"].toMap()["$t"].toString());
    setCountry(address["gd$country"].toMap()["$t"].toString());
    setLocality(address["gd$city"].toMap()["$t"].toString());
    setPostalCode(address["gd$postcode"].toMap()["$t"].toString());
    setRegion(address["gd$region"].toMap()["$t"].toString());
    setPostOfficeBox(address["gd$pobox"].toMap()["$t"].toString());
  }
  
  setType(schemeToType(scheme, primary));
}

Address::Address(const QDomElement& address, const QString& scheme, const bool primary)
{
  QDomNode em = address.elementsByTagName("gd:street").at(0);
  setStreet(em.toElement().text());
  em = address.elementsByTagName("gd:country").at(0);
  setCountry(em.toElement().text());
  em = address.elementsByTagName("gd:city").at(0);
  setLocality(em.toElement().text());
  em = address.elementsByTagName("gd:postcode").at(0);
  setPostalCode(em.toElement().text());
  em = address.elementsByTagName("gd:region").at(0);
  setRegion(em.toElement().text());
  em = address.elementsByTagName("gd:pobox").at(0);
  setPostOfficeBox(em.toElement().text());

  setType(schemeToType(scheme, primary));
}

Address::Address(const KABC::Address& address):
  KABC::Address(address)
{ }

bool Address::operator==(const Address& other)
{
  return ((street() == other.street()) &&
	  (country() == other.country()) &&
	  (locality() == other.locality()) &&
	  (postalCode() == other.postalCode()) &&
	  (region() == other.region()) &&
	  (postOfficeBox() == other.postOfficeBox()) &&
	  (type() == other.type()));
}


Address::Address(const QString& address, const KABC::Address::Type type)
{
  setExtended(address);
  setType(type);
}

QString Address::typeToScheme(const KABC::Address::Type type, bool* primary)
{
  QString typeName;
  
  if (primary)
    *primary = (type & KABC::Address::Pref);
  
  if (type & KABC::Address::Work)
    typeName = "work";
  else if (type & KABC::Address::Home)
    typeName = "home";
  else
    typeName = "other";
  
  return SCHEME_URL + typeName;
}


KABC::Address::Type Address::schemeToType(const QString scheme, const bool primary)
{
  QString typeName = scheme.mid(scheme.lastIndexOf("#")+1);
  KABC::Address::Type type;
  
  if (typeName == "work")
    type = KABC::Address::Work;
  else
    type = KABC::Address::Home;
  
  if (primary)
    type |= KABC::Address::Pref;
  
  return type;
}




/********************************** PHONE NUMBER **********************************/

PhoneNumber::PhoneNumber(const QString& number, const QString& scheme)
{
  setNumber(number);
  setType(schemeToType(scheme));
}

PhoneNumber::PhoneNumber(const QString& number, const KABC::PhoneNumber::Type type)
{
  setNumber(number);
  setType(type);
}

PhoneNumber::PhoneNumber(const KABC::PhoneNumber &phoneNumber):
  KABC::PhoneNumber(phoneNumber)
{ }  

bool PhoneNumber::operator==(const PhoneNumber& other)
{
  return ((number() == other.number()) &&
	  (type() == other.type()));
}


QString PhoneNumber::typeToScheme(const KABC::PhoneNumber::Type type)
{
  QString typeName;
  
  if (type == (KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell))
    typeName = "work_mobile";
  else if (type == (KABC::PhoneNumber::Work | KABC::PhoneNumber::Fax))
    typeName = "work_fax";
  else if (type == (KABC::PhoneNumber::Work | KABC::PhoneNumber::Pager))
    typeName = "work_pager";
  else if (type == (KABC::PhoneNumber::Work | KABC::PhoneNumber::Pref))
    typeName = "company_main";
  else if (type == KABC::PhoneNumber::Work)
    typeName = "work";
  else if (type == (KABC::PhoneNumber::Home | KABC::PhoneNumber::Fax))
    typeName = "home_fax";
  else if (type == KABC::PhoneNumber::Home)
    typeName = "home";
  else if (type == KABC::PhoneNumber::Pref)
    typeName = "main";
  else if (type & KABC::PhoneNumber::Car)
    typeName = "car";
  else if (type & KABC::PhoneNumber::Isdn)
    typeName = "isdn";
  else if (type & KABC::PhoneNumber::Fax)
    typeName = "fax";
  else if (type & KABC::PhoneNumber::Cell)
    typeName = "mobile";
  else if (type & KABC::PhoneNumber::Pager)
    typeName = "pager";
  
  return SCHEME_URL + typeName;
}

KABC::PhoneNumber::Type PhoneNumber::schemeToType(const QString& scheme)
{
  QString typeName = scheme.mid(scheme.lastIndexOf("#")+1);
  KABC::PhoneNumber::Type type;
  
  if (typeName == "car")
    type = KABC::PhoneNumber::Car;
  else if (typeName == "fax")
    type = KABC::PhoneNumber::Fax;
  else if (typeName == "isdn")
    type = KABC::PhoneNumber::Isdn;
  else if (typeName == "mobile")
    type = KABC::PhoneNumber::Cell;
  else if (typeName == "pager")
    type = KABC::PhoneNumber::Pager;
  else if (typeName == "main")
    type = KABC::PhoneNumber::Pref;
  else if (typeName == "home")
    type = KABC::PhoneNumber::Home;
  else if (typeName == "home_fax")
    type = KABC::PhoneNumber::Home | KABC::PhoneNumber::Fax;
  else if (typeName == "work")
    type = KABC::PhoneNumber::Work;
  else if (typeName == "work_fax")
    type = KABC::PhoneNumber::Work | KABC::PhoneNumber::Fax;
  else if (typeName == "work_mobile")
    type = KABC::PhoneNumber::Work | KABC::PhoneNumber::Cell;
  else if (typeName == "work_pager")
    type = KABC::PhoneNumber::Work | KABC::PhoneNumber::Pager;
  else if (typeName == "company_main")
    type = KABC::PhoneNumber::Work | KABC::PhoneNumber::Pref;
  else 
    type =KABC::PhoneNumber::Home;
  
  return type;
}
