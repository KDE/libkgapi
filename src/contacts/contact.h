/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LIBKGAPI2_CONTACT_H
#define LIBKGAPI2_CONTACT_H

#include "object.h"
#include "types.h"
#include "kgapicontacts_export.h"

#include <KContacts/Addressee>
#include <QDateTime>

namespace KGAPI2
{

/**
 * @brief Represents a single contact
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KGAPICONTACTS_EXPORT Contact : public KGAPI2::Object,
                                 public KContacts::Addressee
{
  public:

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
    explicit Contact(const KContacts::Addressee &other);

    /**
     * @brief Copy constructor
     */
    Contact(const Contact &other);


    /**
     * @brief Destructor
     */
    ~Contact();

    bool operator==(const Contact &other) const;

    /**
    * @brief Marks contact as deleted
    *
    * @param deleted
     */
    void setDeleted(bool deleted);

    /**
     * @brief Returns whether contact is deleted
     */
    bool deleted() const;

    /**
     * Sets URL of contact photo.
     *
     * @param photoUrl
     */
    void setPhotoUrl(const QString &photoUrl);

    /**
     * Sets URL of contact photo.
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
    void setCreated(const QDateTime &created);

    /**
     * @brief Returns date and time when the contact was created.
     */
    QDateTime created() const;

    /**
     * @brief Sets date and time or last modification
     *
     * @param updated
     */
    void setUpdated(const QDateTime &updated);

    /**
     * @brief Returns date and time of last modification
     */
    QDateTime updated() const;

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
    static QString phoneTypeToScheme(const KContacts::PhoneNumber::Type type);

    /**
     * @brief Converts Google Scheme URL to phone number type.
     *
     * @param scheme
     */
    static KContacts::PhoneNumber::Type phoneSchemeToType(const QString &scheme);

    /**
     * @brief Converts address type to Google Scheme URL.
     *
     * @param type
     * @param primary
     */
    static QString addressTypeToScheme(const KContacts::Address::Type type, bool *primary = nullptr);

    /**
     * @brief Converts Google Scheme UrL to address type.
     *
     * @param scheme
     * @param primary
     */
    static KContacts::Address::Type addressSchemeToType(const QString &scheme, bool primary = false);

    /**
     * @brief Contacts Google scheme URL to email type.
     */
    static QString emailSchemeToProtocolName(const QString &scheme);

  private:
    class Private;
    Private * const d;

};

} // namespace KGAPI2

#endif /* LIBKGAPI2_CONTACT_H */
