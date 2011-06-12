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

#ifndef CONTACT_H
#define CONTACT_H

#include <KDE/KABC/Addressee>
#include <KDE/KABC/Address>

#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>
#include <QtXml/QDomElement>

namespace Contact {
  
class ContactPrivate;

/**
 * @brief Comfortible class for handling email addresses
 */
class Email
{
  public:
    /**
     * @brief Comfortible datatype for passing list of emails
     */
    typedef QList<Email> List;
        
    /**
     * @brief Creates new emails with #address and specify if it is
     * 	      #primary address or not.
     * 
     * @param address Email address
     * @param primary Specifies wheter #address is primary or notes
     */
    Email(const QString address, const bool primary = false);
    
    /**
     * @brief Sets email address
     * 
     * @param address Email address
     */
    void setAddress(const QString &address);
    
    /**
     * @brief Returns email address
     * 
     * @return Email address
     */
    QString address();
    
    /**
     * @brief Sets wheter the email address is primary or notes
     * 
     * @param primary True when the email address is primary address
     */ 
    void setPrimary(const bool primary);
    
    /**
     * @brief Returns wheter the adddress is primary or notes
     */
    bool primary();
    
  private:
    QString m_address;
    bool m_primary;    
};


/**
 * @brief Data type for handling IM contacts
 */
class IM 
{
  public:

    /**
     * @brief Comfortible type for listof IMs
     */
    typedef QList<IM> List;
    
    /**
     * @brief IM protocol
     */
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
     * @brief Constructs new IM with #address and #protocol.
     * 
     * @param address Address or another identifier used by particular #protocol
     * @param protocol IM protocol
     */
    IM(const QString &address, const IMProtocol protocol);
    
    /**
     * @brief Comfortible method for constructing new IM with #address and #scheme.
     * 
     * Google API provides only scheme URL of protocol, which looks like this:
     * http://schemas.google.com/g/2005#PROTOCOLNAME
     * 
     * @param address Address or any other identfier used by particular #protocol
     * @param protocol Protocol scheme URL
     */
    IM(const QString &address, const QString &scheme);
  
    /**
     * @brief Sets IM address
     * 
     * @param address IM address or aany other identifier used by particular #protocol
     */
    void setAddress(const QString &address);
    
    /**
     * @brief Returns IM address
     * 
     * @return IM address or any other identifier used by particular #protocol
     */
    QString address();
    
    /**
     * @brief Sets scheme
     * 
     * @param scheme Scheme URL for protocol (see @IM(const QString &address, const QString &scheme)).
     */
    void setScheme(const QString &scheme);
    
    /**
     * @brief Returns scheme URL for IM protocol
     * 
     * @returns IM protocol scheme URL
     */
    QString scheme();
    
    /**
     * @brief Sets IM protocol
     * 
     * @param protocol IM protocol
     */
    void setProtocol(const IMProtocol protocol);

    /**
     * @brief Returns IM protocol typedef
     * 
     * @return IM protocol typedef
     */
    IMProtocol protocol();
    
    /**
     * @brief Function for conversion from IM protocol type
     * 	      to GData protocol Scheme URL
     * 
     * @param protocol IM protocol type
     * @return GData Scheme URL for #protocol
     */
    static QString protocolToScheme(const IMProtocol protocol);
    
    /**
     * @brief Function for conversion from GData Scheme URL to 
     * 	      IM protocol type.
     * 
     * @param scheme GData Scheme URL of protocol
     * @return Protocol type
     */
    static IMProtocol schemeToProtocol(const QString &scheme);
    
  private:
    QString m_address;
    QString m_scheme;
    IMProtocol m_protocol;
};


/**
 * @brief Comfortible class for phone numbers.
 */
class PhoneNumber : public KABC::PhoneNumber
{
  public:
    /**
     * @brief Comfortible data type for list of PhoneNumbers
     */
    typedef QList< PhoneNumber > List;
    
    /**
     * @brief Constructs new phone number with #number and type
     * 	      specified by #scheme.
     * 
     * @param number Phone number
     * @param scheme GData Scheme URL
     * @param primary Wheter the phone number is a primary number
     */
    PhoneNumber(const QString &number, const QString &scheme, const bool primary = false);
    
    /**
     * @brief Overloaded contructor to create a new number
     * 
     * @param number Phone number
     * @param type A phone number type
     */
    PhoneNumber(const QString &number, const KABC::PhoneNumber::Type type);
    
    /**
     * @brief Static function for conversion from KABC::PhoneNumber::Type
     * 	      to GData Scheme URL
     * @param type Phone number type
     * @param[out] primary Returns wheter number os a primary number or not
     * @return GData Scheme URL
     */
    static QString typeToScheme(const KABC::PhoneNumber::Type type, bool *primary = 0);
    
    /**
     * @brief Static function for conversion from GData Scheme URL to
     * 	      phone number type.
     * 
     * @param scheme GData Scheme URL
     * @param primary Wheter number is primary or not
     * @return Phone number type
     */
    static KABC::PhoneNumber::Type schemeToType(const QString &scheme, const bool primary = false);
};


class Address : public KABC::Address
{
  public:
    /**
     * @brief Comfortible type for list of Addresses
     */
    typedef QList< Address > List;
    
    /**
     * @brief Construct a new address with #address of type 
     * 	      specified by #scheme
     * 
     * @param address Address
     * @param scheme GData Scheme URL
     * @param primary Wheter the adress is primary or not
     */
    Address(const QString &address, const QString &scheme, const bool primary = false);
    
    /**
     * @brief Comfortible overloaded constructor of an Address
     * 
     * @param address Address
     * @param type Address type
     */
    Address(const QString &address, const KABC::Address::Type type);
    
    /**
     * @brief Static function for conversion from KABC::Address:Type
     * 	      to GData Scheme URL
     * 
     * @param Address Address Type
     * @param[out] primary Boolean value to be set to true when address is primary, or NULL
     * @return GData Scheme URL
     */
    static QString typeToScheme(const KABC::Address::Type type, bool *primary = 0);
    
    /**
     * @brief Static function for conversion from GData Scheme URL
     * 	      to address type.
     * 
     * @param scheme GData Scheme URL
     * @param primary Wheter the address is primary or not
     * @return Address type
     */
    static KABC::Address::Type schemeToType(const QString scheme, const bool primary);
};

class Contact : public QObject
{
  Q_OBJECT
  public:
    /**
     * @brief Shared pointer
     */
    typedef QSharedPointer<Contact> Ptr;
   
    /**
     * @brief Create a new Contact
     */
    Contact();
    
    /**
     * @brief Copy constructor
     */
    Contact(const Contact &other);
    
    /**
     * @brief Destructor
     */
    ~Contact();
    
    /**
     * @brief Parsers and stores data from #jsonData
     * 
     * @param jsonData Data to parse and stores
     */
    void fromJSON(const QVariantMap &jsonData);

    /**
     * @brief Returns content of this object encoded in JSON
     * 
     * @return JSON representation of #Contact
     */
    QVariantMap toJSON();
    
    /**
     * @brief Parses and stores data from #xmlData.
     * 
     * @param xmlData Data to parse and store.
     */
    void fromXML(const QDomElement &xmlData);
    
    /**
     * @brief Returns contact of this object encode in XML
     * 	      format.
     * 
     * @return XML representation of #Contact.
     */
    QByteArray toXML();
    
    /**
     * @brief Parses and stores data from #addressee
     * 
     * @param addressee Data to parse and store
     */ 
    void fromKABC(const KABC::Addressee *addressee);
    
    /**
     * @brief Retruns KABC::Addressee populated by data from this object
     * 
     * @return Populated KABC::Addressee
     */
    KABC::Addressee* toKABC();

    /**
     * @brief Tag contact as deleted.
     */
    void setDeleted(const bool deleted);
    
    /**
     * @brief Returns wheter contact was deleted on server.
     * 
     * @return Returns TRUE when contact was deleted on remote server
     * 	       and should be removed locally too
     */
    bool deleted();
    
    /**
     * @brief Sets URL to fetch contact photo from
     * 
     * @param photoUrl URL to fetch the photo from
     */
    void setPhotoUrl(const QString &photoUrl);
    
    /**
     * @brief Overloaded method
     */
    void setPhotoUrl(const QUrl &photoUrl);
    
    /**
     * @brief Returns URL for fetching photo
     * 
     * @return Returns URL of contact photo
     */
    QUrl photoUrl();
    
    /**
     * @brief Sets unique identificator of contact on remote server.
     * 
     * @param id Unique ID of contact
     */ 
    void setId(const QString &id);
    
    /**
     * @brief Returns unique ID of contact
     * 
     * @return Returns unique ID of contact on remote server
     */
    QString id();
    
    /**
     * @brief Sets ETag which represents current revision
     * 	      of object on remote server.
     * 
     * @param etag Etag string
     */
    void setETag(const QString &etag);
    
    /**
     * @brief Returns ETag of current object
     * 
     * @return Etag
     */
    QString etag();
    
    /**
     * @brief Sets contact's name
     * 
     * @param name Contact's name
     */
    void setName(const QString &name);
    
    /**
     * @brief Returns contact's name
     * 
     * @return Returns contact's name
     */
    QString name();
    
    /**
     * @brief Sets contact's notes
     * 
     * @param notes Contact's notes
     */
    void setNotes(const QString &notes);
    
    /**
     * @brief Returns contact's notes
     * 
     * @return Contact's notes
     */
    QString notes();
    
    /**
     * @brief Sets contact's work (name of work)
     * 
     * To specify address of work, see @addAddress()
     * 
     * @param job Name of company/organization the contact is working for
     */
    void setJob(const QString &job);
    
    /**
     * @brief Returns name of company or organization.
     * 
     * @return Name of company or organization where the contact works.
     */
    QString job();
    
    /**
     * @brief Sests contact's job title
     * 
     * @param jobTitle Job title
     */
    void setJobTitle(const QString &jobTitle);
    
    /**
     * @brief Returns contact's job title
     * 
     * @return Job title
     */
    QString jobTitle();
    
    /**
     * @brief Adds contact's email
     * 
     * @param email QPair of QString-bool, speciying address and wheter
     * 		    it is a primary address or not.
     */
    void addEmail(const Email &email);
    
    /**
     * @brief Returns list of contact's emails
     * 
     * @return List of contact emails
     */
    Email::List emails();
    
    /**
     * @brief Add a new IM
     * 
     * @param im A new im
     */
    void addIM(const IM &im);
    
    /**
     * @brief Returns list of IMs
     * 
     * @return List of IMs
     */
    IM::List IMs();
    
    /**
     * @brief Adds a new phone number
     * 
     * @param phoneNumber A new phone number.
     *
     */
    void addPhoneNumber(const PhoneNumber &phoneNumber);
    
    /**
     * @brief Overloaded method to add a new phone number
     * 
     * @param phoneNumber A new phone number.
     */
    void addPhoneNumber(const KABC::PhoneNumber &phoneNumber);
    
    /**
     * @brief Returns list of phone numbers
     * 
     * @return List of phone numbers
     */
    PhoneNumber::List phoneNumbers();
    
    /**
     * @brief Adds a new address
     * 
     * @param address A new address
     */
    void addAddress(const Address &address);
    
    /**
     * @brief Overloaded method for adding a new address
     * 
     * @param address A new address
     */
    void addAddress(const KABC::Address &address);
    
    /**
     * @brief Returns list of addresses
     * 
     * @return List of addresses
     */
    Address::List addresses();
 
  private:
    QSharedDataPointer<ContactPrivate> d;
};

}; /* Namespace Contact */

Q_DECLARE_METATYPE (Contact::Contact::Ptr)

#endif /* CONTACT_H */