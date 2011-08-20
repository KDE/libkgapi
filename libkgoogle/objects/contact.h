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

    class LIBKGOOGLE_EXPORT Email
    {
      public:
	typedef QList<Email> List;
	    
	Email(const QString address, const bool primary = false);

	void setAddress(const QString &address);
	QString address();
	
	void setPrimary(const bool primary);
	bool primary();
	
	bool operator==(const Email &other);
	
      private:
	QString m_address;
	bool m_primary;    
    };


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
	
	IM(const QString &address, const IMProtocol protocol);
	IM(const QString &address, const QString &scheme);
      
	void setAddress(const QString &address);
	QString address();
	
	void setScheme(const QString &scheme);
	QString scheme();
	
	void setProtocol(const IMProtocol protocol);
	IMProtocol protocol();
	
	static QString protocolToScheme(const IMProtocol protocol);
	static IMProtocol schemeToProtocol(const QString &scheme);
	
	bool operator==(const IM &other);
	
      private:
	QString m_address;
	QString m_scheme;
	IMProtocol m_protocol;
    };


    class LIBKGOOGLE_EXPORT PhoneNumber : public KABC::PhoneNumber
    {
      public:
	typedef QList< PhoneNumber > List;
	
	PhoneNumber(const QString &number, const QString &scheme);
	PhoneNumber(const QString &number, const KABC::PhoneNumber::Type type);
	PhoneNumber(const KABC::PhoneNumber &phoneNumber);

	static QString typeToScheme(const KABC::PhoneNumber::Type type);
	static KABC::PhoneNumber::Type schemeToType(const QString &scheme);
	
	bool operator==(const PhoneNumber &other);
    };


    class LIBKGOOGLE_EXPORT Address : public KABC::Address
    {
      public:
	typedef QList< Address > List;
	
	Address(const QString &street = QString(), const QString &pobox = QString(), const QString &locality = QString(),
		const QString &region = QString(), const QString &postalCode = QString(), const QString &country = QString(),
		const KABC::Address::Type type = KABC::Address::Home);
	Address(const QVariantMap &address, const QString &scheme, const bool primary = false);
	Address(const QDomElement &address, const QString &scheme, const bool primary = false);
	Address(const KABC::Address &address);
	Address(const QString &address, const KABC::Address::Type type);
	
	static QString typeToScheme(const KABC::Address::Type type, bool *primary = 0);
	static KABC::Address::Type schemeToType(const QString scheme, const bool primary);
	
	bool operator==(const Address &other);
    };

    class LIBKGOOGLE_EXPORT Contact : public KGoogleObject
    {
      public:
	typedef QSharedPointer<Contact> Ptr;

	Contact();
	Contact(const Contact &other);
	
	~Contact();

	void setDeleted(const bool deleted);
	bool deleted();

	void setPhotoUrl(const QString &photoUrl);
	void setPhotoUrl(const QUrl &photoUrl);
	QUrl photoUrl();

	void setName(const QString &name);
	QString name();

	void setNotes(const QString &notes);
	QString notes();

	void setJob(const QString &job);
	QString job();

	void setJobTitle(const QString &jobTitle);
	QString jobTitle();

	void setBirthday(const QString &birthday);
	QString birthday();

	void setCreated(const KDateTime &created);
	KDateTime created();
      
	void setUpdated(const KDateTime &updated);
	KDateTime updated();

	void addEmail(const Email &email);
	Email::List emails();

	void addIM(const IM &im);
	IM::List IMs();

	void addPhoneNumber(const PhoneNumber &phoneNumber);
	void addPhoneNumber(const KABC::PhoneNumber &phoneNumber);
	PhoneNumber::List phoneNumbers();

	void addAddress(const Address &address);
	void addAddress(const KABC::Address &address);
	Address::List addresses();

	void fromKABC(const KABC::Addressee *addressee);
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
