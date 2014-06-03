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


#ifndef LIBKGAPI2_CONTACTSGROUP_H
#define LIBKGAPI2_CONTACTSGROUP_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QDateTime>

namespace KGAPI2
{

/**
 * @brief Represents a contacts group.
 *
 * Note that this is not the same as vCard contact groups. ContactsGroup is
 * similar to an addressbook - it contains set of full-fetured Contacts.
 *
 * Unlike regular addressbooks however, a single instance of contact can be
 * present in multiple ContactsGroup.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class LIBKGAPI2_EXPORT ContactsGroup : public KGAPI2::Object
{
  public:

    /**
     * @brief Constructor
     */
    ContactsGroup();

    /**
     * @brief Copy constructor
     */
    ContactsGroup(const ContactsGroup &other);

    /**
     * @brief Destructor
     */
    virtual ~ContactsGroup();

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
    Private * const d;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTSGROUP_H
