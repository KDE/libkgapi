/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_CONTACT_H
#define LIBKGAPI2_CONTACT_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDE/KABC/Addressee>
#include <KDE/KDateTime>

namespace KGAPI2
{

/**
 * @brief Represents a single contact
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class LIBKGAPI2_EXPORT Contact : public KGAPI2::Object,
                                public KABC::Addressee
{
  public:

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef ContactPtr Ptr;

    /**
     * @brief Instant Messaging Protocols
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
     * @brief Constructor
     */
    Contact();

    /**
     * @brief Copy constructor
     */
    explicit Contact(const KABC::Addressee &other);

    /**
     * @brief Copy constructor
     */
    Contact(const Contact &other);

    /**
     * @brief Destructor
     */
    ~Contact();

    /**
    * @brief Marks contact as deleted
    *
    * @param deleted
     */
    void setDeleted(bool deleted);

    /**
     * @param Returns whether contact is deleted
     */
    bool deleted() const;

    /**
     * @param Sets URL of contact photo.
     *
     * @param photoUrl
     */
    void setPhotoUrl(const QString &photoUrl);

    /**
     * @param Sets URL of contact photo.
     * @overload setPhotoUrl(QString)
     */
    void setPhotoUrl(const QUrl &photoUrl);

    /**
     * @brief Returns URL of contact photo.
     */
    QUrl photoUrl() const;

    /**
     * @brief Sets date and time when the contact was created.
     *
     * @param created
     */
    void setCreated(const KDateTime &created);

    /**
     * @brief Returns date and time when the contact was created.
     */
    KDateTime created() const;

    /**
     * @brief Sets date and time or last modification
     *
     * @param updated
     */
    void setUpdated(const KDateTime &updated);

    /**
     * @brief Returns date and time of last modification
     */
    KDateTime updated() const;

    /**
     * @brief Sets name of contact's spouse.
     *
     * @param name
     */
    void setSpousesName(const QString &name);

    /**
     * @brief Returns name of contacts' spouse
     */
    QString spousesName() const;

    /**
     * @brief Sets name of contact's manager.
     *
     * @param name
     */
    void setManagersName(const QString &name);

    /**
     * @brief Returns name of contact's manager.
     */
    QString managersName() const;

    /**
     * @brief Sets assistant's name.
     *
     * @param name
     */
    void setAssistantsName(const QString &name);

    /**
     * @brief Returns name of contact's assistant.
     */
    QString assistantsName() const;

    /**
     * @brief Sets contact's profession.
     *
     * @param profession
     */
    void setProfession(const QString &profession);

    /**
     * @brief Returns contact's profession.
     */
    QString profession() const;

    /**
     * @brief Sets address of contact's office.
     *
     * @param office
     */
    void setOffice(const QString &office);

    /**
     * @brief Returns address of contacts' office.
     */
    QString office() const;

    /**
     * @brief Sets date of contact's anniversary.
     *
     * @param anniversary
     */
    void setAnniversary(const QString &anniversary);

    /**
     * @brief Returns date of contact's anniversary
     */
    QString anniversary() const;

    /**
     * @brief Sets URL of contact's blog.
     *
     * @param url
     */
    void setBlogFeed(const QString &url);

    /**
     * @brief Returns URL of contact's blog.
     */
    QString blogFeed() const;

    /**
     * @brief Set list of groups the contact is member of.
     *
     * @param groups
     */
    void setGroups(const QStringList &groups);

    /**
     * @brief Adds a group the contact is member of
     *
     * @param group
     */
    void addGroup(const QString &group);

    /**
     * @brief Remove contact from a group.
     *
     * Use this method to remove contact from groups, instead
     * of setGroups(QStringList()). To clear group membership
     * use clearGroups()
     *
     * @param group
     */
    void removeGroup(const QString &group);

    /**
     * @brief Removes contact from all groups it's member of.
     */
    void clearGroups();

    /**
     * @brief Returns list of groups the contact is member of.
     */
    QStringList groups() const;

    /**
     * @brief Returns whether contact has been removed from the @p group or not.
     *
     * @param group
     */
    bool groupIsDeleted(const QString &group) const;

    /**
     * @brief Converts IMProtocol to Google Scheme URL.
     *
     * @param protocol
     */
    static QString IMProtocolToScheme(const IMProtocol protocol);

    /**
     * @brief Converts Google Scheme URL to string with protocol name.
     *
     * @param scheme
     */
    static QString IMSchemeToProtocolName(const QString &scheme);

    /**
     * @brief Converts string with protocol name to Google Scheme URL.
     *
     * @param protocolName
     */
    static QString IMProtocolNameToScheme(const QString &protocolName);

    /**
     * @brief Converts Google Scheme URL to IMProtocol.
     *
     * @param scheme
     */
    static IMProtocol IMSchemeToProtocol(const QString &scheme);

    /**
     * @brief Converts phone number type to Google Scheme URL.
     *
     * @param type
     */
    static QString phoneTypeToScheme(const KABC::PhoneNumber::Type type);

    /**
     * @brief Converts Google Scheme URL to phone number type.
     *
     * @parman scheme
     */
    static KABC::PhoneNumber::Type phoneSchemeToType(const QString &scheme);

    /**
     * @brief Converts address type to Google Scheme URL.
     *
     * @param type
     * @param primary
     */
    static QString addressTypeToScheme(const KABC::Address::Type type, bool *primary = 0);

    /**
     * @brief Converts Google Scheme UrL to address type.
     *
     * @param scheme
     * @param primary
     */
    static KABC::Address::Type addressSchemeToType(const QString &scheme, bool primary = false);

  private:
    class Private;
    Private * const d;

};

} // namespace KGAPI2

#endif /* LIBKGAPI2_CONTACT_H */
