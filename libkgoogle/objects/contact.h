/*
    libKGoogle - Objects - Contact
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


#ifndef LIBKGOOGLE_OBJECTS_CONTACT_H
#define LIBKGOOGLE_OBJECTS_CONTACT_H

#include <libkgoogle/object.h>
#include <libkgoogle/libkgoogle_export.h>

#include <kabc/addressee.h>
#include <kdatetime.h>

#include <qlist.h>
#include <qmetatype.h>
#include <qshareddata.h>
#include <qdom.h>

namespace KGoogle {

  namespace Objects {

    class ContactData;

    /**
     * Represents a single contact.
     */
    class LIBKGOOGLE_EXPORT Contact : public KGoogle::Object, public KABC::Addressee
    {
      public:
	typedef QSharedPointer<Contact> Ptr;

	enum IMProtocol {
	  Jabber, ICQ, GoogleTalk, QQ, Skype, Yahoo, MSN, AIM, Other
	};

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
	bool deleted() const;

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
	QUrl photoUrl() const;

	/**
	 * Sets date and time when the contact was created.
	 */
	void setCreated(const KDateTime &created);

	/**
	 * Returns date and time when the contact was created.
	 */
	KDateTime created() const;

	/**
	 * Sets date and time or last change of the contact
	 * on the remote server.
	 */
	void setUpdated(const KDateTime &updated);

	/**
	 * Returns date and time of last change of the contact
	 * on the remote server.
	 */
	KDateTime updated() const;

        /**
         * Sets name of contact's spouse.
         */
        void setSpousesName(const QString &name);

        /**
         * Returns name of contacts' spouse
         */
        QString spousesName() const;

        /**
         * Sets name of contact's manager.
         */
        void setManagersName(const QString &name);

        /**
         * Returns name of contact's manager.
         */
        QString managersName() const;

        /**
         * Sets assistant's name.
         */
        void setAssistantsName(const QString &name);

        /**
         * Returns name of contact's assistant.
         */
        QString assistantsName() const;

        /**
         * Sets contact's profession.
         */
        void setProfession(const QString &profession);

        /**
         * Returns contact's profession.
         */
        QString profession() const;

        /**
         * Sets address of contact's office.
         */
        void setOffice(const QString &office);

        /**
         * Returns address of contacts' office.
         */
        QString office() const;

        /**
         * Sets date of contact's anniversary.
         */
        void setAnniversary(const QString &anniversary);

        /**
         * Returns date of contact's anniversary
         */
        QString anniversary() const;

        /**
         * Sets URL of contact's blog.
         */
        void setBlogFeed(const QString &url);

        /**
         * Returns URL of contact's blog.
         */
        QString blogFeed() const;

	/**
	 * Set list of groups the contact is member of.
	 */
	void setGroups(const QStringList &groups);

	/**
	 * Adds a group the contact is member of
	 */
	void addGroup(const QString &group);

	/**
	 * Returns list of groups the contact is member of.
	 */
	QStringList groups() const;

	/**
	 * Converts IMProtocol to Google Scheme URL.
	 */
	static QString IMProtocolToScheme(const IMProtocol protocol);

	/**
	 * Converts Google Scheme URL to string with protocol name.
	 */
	static QString IMSchemeToProtocolName(const QString &scheme);

	/**
	 * Converts string with protocol name to Google Scheme URL.
	 */
	static QString IMProtocolNameToScheme(const QString &protocolName);

	/**
	 * Converts Google Scheme URL to IMProtocol.
	 */
	static IMProtocol IMSchemeToProtocol(const QString &scheme);

	/**
	 * Converts phone number type to Google Scheme URL.
	 */
	static QString phoneTypeToScheme(const KABC::PhoneNumber::Type type);

	/**
	 * Converts Google Scheme URL to phone number type.
	 */
	static KABC::PhoneNumber::Type phoneSchemeToType(const QString &scheme);

	/**
	 * Converts address type to Google Scheme URL.
	 */
	static QString addressTypeToScheme(const KABC::Address::Type type, bool *primary = 0);

	/**
	 * Converts Google Scheme UrL to address type.
	 */
	static KABC::Address::Type addressSchemeToType(const QString &scheme, const bool primary = false);

      private:
	QExplicitlySharedDataPointer<ContactData> d;

    };

  } // namespace Objects

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Objects::Contact)
Q_DECLARE_METATYPE(KGoogle::Objects::Contact::Ptr)

#endif /* LIBKGOOGLE_OBJECTS_CONTACT_H */



