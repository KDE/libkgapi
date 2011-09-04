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


#ifndef OBJECT_CONTACT_H
#define OBJECT_CONTACT_H

#include "kgoogleobject.h"
#include "kabc/addressee.h"
#include "libkgoogle_export.h"

#include <qlist.h>
#include <qmetatype.h>
#include <qshareddata.h>
#include <qdom.h>

#include <kdatetime.h>

namespace KGoogle {
  
  namespace Object {
  
    class ContactData;

    /**
     * Informations about an email address.
     */
    class LIBKGOOGLE_EXPORT Email
    {
      public:
	typedef QList<Email> List;
	    
	/**
	 * Constructs a new email address.
	 * 
	 * @param address A valid email address.
	 * @param primary Wheter address is primary (preferred).
	 */
	Email(const QString address, const bool primary = false);

	/**
	 * Sets a new email p0 address.
	 */
	void setAddress(const QString &address);
	
	/**
	 * Returns an email address
	 */
	QString address();
	
	/**
	 * Sets wheter the address is primary (preferred).
	 */
	void setPrimary(const bool primary);
	
	/**
	 * Returns wheter the address is primary (preferred).
	 */
	bool primary();
	
	/**
	 * Compares an email to another one.
	 */
	bool operator==(const Email &other);
	
      private:
	QString m_address;
	bool m_primary;    
    };


    /**
     * Informations about an IM contact.
     */
    class LIBKGOOGLE_EXPORT IM 
    {
      public:
	typedef QList<IM> List;

	enum IMProtocol { 
	  Jabber, 
	  ICQ, 
	  GoogleTalk, 
	  QQ, 
	  Skype, 
	  Yahoo, 
	  MSN, 
	  AIM,
	  Other
	};
	
	/**
	 * Constructs a new IM contact.
	 * 
	 * @param address IM address/UID/etc.
	 * @param protocol Protocol type.
	 */
	IM(const QString &address, const IMProtocol protocol);
	
	/**
	 * Constructs a new IM contact.
	 * 
	 * @param address IM address, UID or another user identifier.
	 * @param scheme Google URL scheme representing IM protocol.
	 */
	IM(const QString &address, const QString &scheme);
      
	/**
	 * Sets IM address, UID or another user identifier.
	 */
	void setAddress(const QString &address);
	
	/**
	 * Returns users IM address, UID or another identifier.
	 */
	QString address();
	
	/**
	 * Sets Google scheme URL representing a IM protocol.
	 */
	void setScheme(const QString &scheme);
	
	/**
	 * Returns protocol Google scheme URL.
	 */
	QString scheme();
	
	/**
	 * Sets IM protocol.
	 */
	void setProtocol(const IMProtocol protocol);
	
	/**
	 * Returns IM protocol.
	 */
	IMProtocol protocol();
	
	/**
	 * Converts IMProtocol to Google scheme URL.
	 */
	static QString protocolToScheme(const IMProtocol protocol);
	
	/**
	 * Converts Google scheme URL to IMProtocol.
	 */
	static IMProtocol schemeToProtocol(const QString &scheme);
	
	/**
	 * Compares an IM to another one.
	 */
	bool operator==(const IM &other);
	
      private:
	QString m_address;
	QString m_scheme;
	IMProtocol m_protocol;
    };


    /**
     * Informations about a phone number.
     */
    class LIBKGOOGLE_EXPORT PhoneNumber : public KABC::PhoneNumber
    {
      public:
	typedef QList< PhoneNumber > List;
	
	/**
	 * Constructs a new phone number.
	 * 
	 * @param number A valid phone number.
	 * @param scheme Google scheme URL representing a phone type.
	 */
	PhoneNumber(const QString &number, const QString &scheme);
	
	/**
	 * Constructs a new phone number.
	 * 
	 * @param number A valid phone number.
	 * @param type A phone type.
	 */
	PhoneNumber(const QString &number, const KABC::PhoneNumber::Type type);
	PhoneNumber(const KABC::PhoneNumber &phoneNumber);

	/**
	 * Converts Phone number type to Google scheme URL.
	 */
	static QString typeToScheme(const KABC::PhoneNumber::Type type);
	
	/**
	 * Converts a Google scheme URL to a number type.
	 */
	static KABC::PhoneNumber::Type schemeToType(const QString &scheme);
	
	/**
	 * Compares one PhoneNumber to another.
	 */
	bool operator==(const PhoneNumber &other);
    };


    /**
     * Informations about contact address.
     */
    class LIBKGOOGLE_EXPORT Address : public KABC::Address
    {
      public:
	typedef QList< Address > List;
	
	/**
	 * Constructs a new address
	 * 
	 * @param street Street name and number
	 * @param pobox PO BOX
	 * @param locality Locality (city)
	 * @param region Region
	 * @param postalCode Postal code
	 * @param country Full country name
	 * @param type Address type.
	 */
	Address(const QString &street = QString(), const QString &pobox = QString(), const QString &locality = QString(),
		const QString &region = QString(), const QString &postalCode = QString(), const QString &country = QString(),
		const KABC::Address::Type type = KABC::Address::Home);
	
	/**
	 * Constructs a new address from JSON data.
	 * 
	 * The data should be in following format:
	 * \code
	 * {
	 * 	gd$street: "Street name and number",
	 * 	gd$city: "City (locality)",
	 * 	gd$postCode: "Postal code",
	 * 	gd$region: "Region",
	 * 	gd$pobox: "PO BOX"
	 * }
	 * \endcode
	 * 
	 * @param address A QVariantMap with address.
	 * @param scheme Google scheme URL representing address type.
	 * @param primary Wheter this is the preferred address.
	 */
	Address(const QVariantMap &address, const QString &scheme, const bool primary = false);
	
	/**
	 * Constructs a new address from XML data.
	 * 
	 * The data should be in following format:
	 * \code
	 * <gd:structuredPostalAddress>
	 * 	<gd:street>Street name and number</gd:street>
	 * 	<gd:country>Country</gd:country>
	 * 	<gd:city>City (locality)</gd:city>
	 * 	<gd:postcode>Postal Code</gd:postcode>
	 * 	<gd:region>Region</gd:region>
	 * 	<gd:pobox>PO BOX</gd:pobox>
	 * </gd:structuredPostalAddress>
	 * \endcode
	 * 
	 * @param address QDomElement with address.
	 * @param scheme Google scheme URL representing address type.
	 * @param primary Wheter the address is preferred.
	 */
	Address(const QDomElement &address, const QString &scheme, const bool primary = false);
	
	/**
	 * Constructs a new address from unstructured data.
	 *
	 * @param address Address
	 * @param type Address type.
	 */
	Address(const QString &address, const KABC::Address::Type type);

	/**
	 * A copy constructor.
	 */
	Address(const KABC::Address &address);
	
	/**
	 * Converts address type to Google scheme URL.
	 */
	static QString typeToScheme(const KABC::Address::Type type, bool *primary = 0);
	
	/**
	 * Converts Google scheme URL to address type.
	 */
	static KABC::Address::Type schemeToType(const QString scheme, const bool primary);
	
	/**
	 * Compares one address to another.
	 */
	bool operator==(const Address &other);
    };

    /**
     * Represents a single contact.
     */
    class LIBKGOOGLE_EXPORT Contact : public KGoogleObject
    {
      public:
	typedef QSharedPointer<Contact> Ptr;

	/**
	 * Constructs a new contact.
	 */
	Contact();
	Contact(const Contact &other);
	
	~Contact();

	/**
	 * Tags contact as deleted on the remote server.
	 */
	void setDeleted(const bool deleted);
	
	/**
	 * Returns wheter contact was deleted on the remote server.
	 */
	bool deleted();

	/**
	 * Sets URL of contact photo.
	 */
	void setPhotoUrl(const QString &photoUrl);
	
	/**
	 * Sets URL of contact photo.
	 */
	void setPhotoUrl(const QUrl &photoUrl);
	
	/**
	 * Returns URL of contact photo.
	 */
	QUrl photoUrl();

	/**
	 * Sets contact name.
	 */
	void setName(const QString &name);
	
	/**
	 * Returns contact name.
	 */
	QString name();

	/**
	 * Sets additional notes about contact.
	 */
	void setNotes(const QString &notes);
	
	/**
	 * Returns additinal information about contact.
	 */
	QString notes();

	/**
	 * Sets name of a company or employer Contact is 
	 * working for.
	 */
	void setJob(const QString &job);
	
	/**
	 * Returns name of a company or employer Contact is
	 * working for.
	 */
	QString job();

	/**
	 * Sets name of job Contact does.
	 */
	void setJobTitle(const QString &jobTitle);
	
	/**
	 * Returns name of Contact's job.
	 */
	QString jobTitle();

	/**
	 * Sets birthday date. 
	 * 
	 * @param birthday Birtday date in format YYYY-MM-DD.
	 */
	void setBirthday(const QString &birthday);
	
	/**
	 * Returns birthday date in format YYYY-MM-DD.
	 */
	QString birthday();

	/**
	 * Sets date and time when the contact was created.
	 */
	void setCreated(const KDateTime &created);
	
	/**
	 * Returns date and time when the contact was created.
	 */
	KDateTime created();
      
	/**
	 * Sets date and time or last change of the contact
	 * on the remote server.
	 */
	void setUpdated(const KDateTime &updated);
	
	/**
	 * Returns date and time of last change of the contact
	 * on the remote server.
	 */
	KDateTime updated();

	/**
	 * Adds an email contact.
	 */
	void addEmail(const Email &email);
	
	/**
	 * Returns list of email contacts.
	 */
	Email::List emails();

	/**
	 * Adds an IM contact.
	 */
	void addIM(const IM &im);
	
	/**
	 * Returns list of IM contacts.
	 */
	IM::List IMs();
	
	/**
	 * Adds a phone number.
	 */
	void addPhoneNumber(const PhoneNumber &phoneNumber);
	
	/** 
	 * Adds a phone number.
	 */
	void addPhoneNumber(const KABC::PhoneNumber &phoneNumber);
	
	/**
	 * Returns list of phone numbers
	 */
	PhoneNumber::List phoneNumbers();

	/**
	 * Adds an address.
	 */
	void addAddress(const Address &address);
	
	/**
	 * Adds an address.
	 */
	void addAddress(const KABC::Address &address);
	
	/**
	 * Returns list of addresses.
	 */
	Address::List addresses();

	/**
	 * Additional parser that will populate this class
	 * by data from KABC::Addressee object.
	 */
	void fromKABC(const KABC::Addressee *addressee);
	
	/**
	 * Converts this object into a KABC::Addressee object.
	 */
	KABC::Addressee* toKABC();

      private:
	QExplicitlySharedDataPointer<ContactData> d;

    };
    
  } // namespace Object

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Object::Contact)
Q_DECLARE_METATYPE(KGoogle::Object::Contact::Ptr)
Q_DECLARE_METATYPE(KGoogle::Object::Email::List)
Q_DECLARE_METATYPE(KGoogle::Object::IM::List)
Q_DECLARE_METATYPE(KGoogle::Object::PhoneNumber::List)
Q_DECLARE_METATYPE(KGoogle::Object::Address::List)

#endif /* OBJECT_CONTACT_H */
