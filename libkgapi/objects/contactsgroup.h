/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_OBJECTS_CONTACTSGROUP_H
#define LIBKGAPI_OBJECTS_CONTACTSGROUP_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include <QtCore/QMetaType>

namespace KGAPI
{

namespace Objects
{

class ContactsGroupPrivate;

/**
 * Represents a single contacts group.
 *
 * Unlike groups in KDE (a meta-contact with list of emails and names),
 * groups in Google represent a collection of contacts.
 */
class LIBKGAPI_EXPORT ContactsGroup : public KGAPI::Object
{
  public:
    typedef QSharedPointer< ContactsGroup > Ptr;

    ContactsGroup();

    ContactsGroup(const ContactsGroup &other);

    virtual ~ContactsGroup();

    /**
     * Sets group ID
     *
     * @param id ID of group.
     */
    void setId(const QString &id);

    /**
     * Returns group ID.
     */
    QString id() const;

    /**
     * Sets group name.
     *
     * @param title Group name.
     */
    void setTitle(const QString &title);

    /**
     * Returns group name.
     */
    QString title() const;

    /**
     * Sets description what kind of contacts the group contains.
     *
     * @param content Description of group content.
     */
    void setContent(const QString &content);

    /**
     * Returns description of group content.
     */
    QString content() const;

    /**
     * Sets when was the group updated last time.
     *
     * @param updated Date and time of last update.
     */
    void setUpdated(const QDateTime &updated);

    /**
     * Returns date and time of last update.
     */
    QDateTime updated() const;

    /**
     * Sets whether this group is a system group.
     *
     * The system groups (Family, Co-workers, etc.)are present in all
     * addressbooks.
     *
     * Default value is false.
     *
     * @param isSystemGroup Whether group is the system group.
     */
    void setIsSystemGroup(const bool isSystemGroup);

    /**
     * Returns whether the group is a system group..
     */
    bool isSystemGroup() const;

  private:
    QSharedDataPointer< ContactsGroupPrivate > d;

};

} /* namespace Objects */

} /* namespace KGAPI */

Q_DECLARE_METATYPE(KGAPI::Objects::ContactsGroup::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::ContactsGroup*)

#endif // LIBKGAPI_OBJECTS_CONTACTSGROUP_H
