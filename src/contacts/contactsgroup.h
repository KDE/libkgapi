/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#ifndef LIBKGAPI2_CONTACTSGROUP_H
#define LIBKGAPI2_CONTACTSGROUP_H

#include "object.h"
#include "types.h"
#include "kgapicontacts_export.h"

#include <QDateTime>

namespace KGAPI2
{

/**
 * @brief Represents a contacts group.
 *
 * Note that this is not the same as vCard contact groups. ContactsGroup is
 * similar to an addressbook - it contains set of full-featured Contacts.
 *
 * Unlike regular addressbooks however, a single instance of contact can be
 * present in multiple ContactsGroup.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KGAPICONTACTS_EXPORT ContactsGroup : public KGAPI2::Object
{
public:

    /**
     * @brief Constructor
     */
    explicit ContactsGroup();

    /**
     * @brief Destructor
     */
    ~ContactsGroup() override;

    /**
     * @brief Compares the groups
     */
    bool operator==(const ContactsGroup &other) const;

    /**
     * @brief Sets group ID
     *
     * @param id
     */
    void setId(const QString &id);

    /**
     * @brief Returns group ID.
     */
    QString id() const;

    /**
     * @brief Sets group name.
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief Returns group name.
     */
    QString title() const;

    /**
     * @brief Sets description what kind of contacts the group contains.
     *
     * @param content
     */
    void setContent(const QString &content);

    /**
     * @brief Returns description of group content.
     */
    QString content() const;

    /**
     * @brief Sets when was the group updated last time.
     *
     * @param updated
     */
    void setUpdated(const QDateTime &updated);

    /**
     * @brief Returns date and time of last update.
     */
    QDateTime updated() const;

    /**
     * @brief Sets whether this group is a system group.
     *
     * The system groups (Family, Co-workers, etc.) are present in all
     * addressbooks.
     *
     * Default value is false.
     *
     * @param isSystemGroup
     */
    void setIsSystemGroup(const bool isSystemGroup);

    /**
     * @brief Returns whether the group is a system group.
     */
    bool isSystemGroup() const;

private:
    class Private;
    QScopedPointer<Private> const d;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTSGROUP_H
