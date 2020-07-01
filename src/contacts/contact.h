/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
