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

#ifndef LIBKGAPI2_CONTACTSSERVICE_H
#define LIBKGAPI2_CONTACTSSERVICE_H

#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi_export.h>

#include <QtCore/QUrl>

namespace KGAPI2
{

/**
 * @brief Additional methods for implementing support for Google Contacts service
 *
 * You should never need to use these methods, unless implementing your own Job
 *
 * There are no methods to serialize objects into JSON, because Google Contacts
 * API only supports XML for writing.
 */
namespace ContactsService
{

    /**
     * @brief Parses JSON data into Contact object
     *
     * @param jsonData
     */
    LIBKGAPI_EXPORT ContactPtr JSONToContact(const QByteArray& jsonData);

    /**
     * @brief Parses JSON data into ContactsGroup object
     *
     * @param jsonData
     */
    LIBKGAPI_EXPORT ContactsGroupPtr JSONToContactsGroup(const QByteArray& jsonData);

    /**
     * @brief Parses JSON feed into list of ContactsGroups or Events.
     *
     * @param jsonFeed
     * @param feedData The structure will be filled with additional data
     */
    LIBKGAPI_EXPORT ObjectsList parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * @brief Parses XML data into Contact object
     *
     * @param xmlData
     */
    LIBKGAPI_EXPORT ContactPtr XMLToContact(const QByteArray& xmlData);

    /**
     * @brief Serializes a Contact object into XML
     *
     * @param contact
     */
    LIBKGAPI_EXPORT QByteArray contactToXML(const ContactPtr& contact);

    /**
     * @brief Parses XML data into ContactsGroup object
     *
     * @param xmlData
     */
    LIBKGAPI_EXPORT ContactsGroupPtr XMLToContactsGroup(const QByteArray& xmlData);

    /**
     * @brief Serializes a ContactsGroup object into XML
     *
     * @param group
     */
    LIBKGAPI_EXPORT QByteArray contactsGroupToXML(const ContactsGroupPtr& group);

    /**
     * @brief Supported API version
     */
    LIBKGAPI_EXPORT QString APIVersion();

    /**
     * @brief Returns scope URL for Google Calendar service.
     *
     * https://www.google.com/m8/feeds/
     */
    LIBKGAPI_EXPORT QUrl scopeUrl();

    /**
     * @brief Returns URL for creating contacts
     *
     * @param user User. "default" or user@gmail.com}
     */
    LIBKGAPI_EXPORT QUrl createContactUrl(const QString &user);

    /**
     * @brief Returns URL for fetching all contacts in addressbook
     *
     * @param user User. "default" or user@gmail.com
     * @param showDeleted Whether to fetch deleted contacts placeholders as well
     */
    LIBKGAPI_EXPORT QUrl fetchAllContactsUrl(const QString &user, const bool &showDeleted);

    /**
     * @brief Returns URL for fetching specific contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to fetch
     */
    LIBKGAPI_EXPORT QUrl fetchContactUrl(const QString &user, const QString &contactID);

    /**
     * @brief Returns URL for updating a single contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to update
     */
    LIBKGAPI_EXPORT QUrl updateContactUrl(const QString &user, const QString &contactID);

    /**
     * @brief Returns URL for removing a single contact
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to remove
     */
    LIBKGAPI_EXPORT QUrl removeContactUrl(const QString &user, const QString &contactID);

    /**
     * Returns URL for KGAPI::Request::FetchAll requests.
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look.
     */
    LIBKGAPI_EXPORT QUrl fetchAllGroupsUrl(const QString &user);

    /**
     * @brief Returns URL for fetching all contacts groups
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look
     * @param groupId ID of group of which to fetch all data.
     */
    LIBKGAPI_EXPORT QUrl fetchGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for creating a new contacts group
     *
     * @param user User (user@gmail.com) in whose addressbook to create a group.
     */
    LIBKGAPI_EXPORT QUrl createGroupUrl(const QString &user);

    /**
     * @brief Returns URL for updating a single contacts group
     *
     * @param user User (user@gmail.com)) in whose addressbook to look
     * @param groupId ID of group to update.
     */
    LIBKGAPI_EXPORT QUrl updateGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for removing a single contacts group
     *
     * @param user User (user@gmail.com) in whose addressbook to look
     * @param groupId ID of group to delete
     */
    LIBKGAPI_EXPORT QUrl removeGroupUrl(const QString &user, const QString &groupId);

    /**
     * @brief Returns URL for modifying photo.
     *
     * Send a GET request to fetch the photo, PUT to change it and DELETE to
     * remove it.
     *
     * @param user User (user@gmail.com) in whose addressbook to lok
     * @param contactId ID of contacts whose photo to update
     */
    LIBKGAPI_EXPORT QUrl photoUrl(const QString &user, const QString &contactId);

} // namespace ContactsService

} // namespace KGAPI

#endif // LIBKGAPI2_CONTACTSSERVICE_H
