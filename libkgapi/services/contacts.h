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

#ifndef LIBKGAPI_SERVICES_CONTACTS_H
#define LIBKGAPI_SERVICES_CONTACTS_H

#include <libkgapi/service.h>
#include <libkgapi/libkgapi_export.h>

#include <qmetatype.h>
#include <qdom.h>

namespace KGAPI
{

class Object;

namespace Services
{

class ContactsPrivate;

/**
 * Represents Google Contacts service.
 */
class LIBKGAPI_EXPORT Contacts: public KGAPI::Service
{
public:
    static QUrl ScopeUrl;

    /**
     * Implementation of KGAPI::Service::JSONToObject().
     */
    KGAPI::Object* JSONToObject(const QByteArray& jsonData);

    /**
     * Implementation of KGAPI::Service::objectToJSON().
     */
    QByteArray objectToJSON(KGAPI::Object* object);

    /**
     * Implementation of KGAPI::Service::parseJSONFeed().
     */
    QList< KGAPI::Object* > parseJSONFeed(const QByteArray& jsonFeed, FeedData& feedData);

    /**
     * Implementation of KGAPI::Service::XMLToObject().
     */
    KGAPI::Object* XMLToObject(const QByteArray& xmlData);

    /**
     * Implementation of KGAPI::Service::objectToXML().
     */
    QByteArray objectToXML(KGAPI::Object* object);

    /**
     * Implementation of KGAPI::Service::parseXMLFeed().
     */
    QList< KGAPI::Object* > parseXMLFeed(const QByteArray& xmlFeed, FeedData& feedData);

    /**
     * Implementation of KGAPI::Service::protocolVersion().
     */
    QString protocolVersion() const;

    /**
     * Returns scope URL for Google Calendar service.
     *
     * https://www.google.com/m8/feeds/
     */
    const QUrl& scopeUrl() const;

    /**
     * Returns URL for KGAPI::Request::Create requests.
     *
     * @param user User. "default" or user@gmail.com}
     */
    static QUrl createContactUrl(const QString &user);

    /**
     * Returns URL for KGAPI::Request::FetchAll requests.
     *
     * @param user User. "default" or user@gmail.com
     * @param showDeleted Whether to fetch deleted contacts placeholders as well
     */
    static QUrl fetchAllContactsUrl(const QString &user, const bool &showDeleted);

    /**
     * Returns URL for KGAPI::Request::Fetch requests.
     *
     * https://www.google.com/m8/feeds/contacts/%1/full/%2
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to fetch
     */
    static QUrl fetchContactUrl(const QString &user, const QString &contactID);

    /**
     * Returns URL for KGAPI::Request::Update requests.
     *
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to update
     */
    static QUrl updateContactUrl(const QString &user, const QString &contactID);

    /**
     * Returns URL for KGAPI::Request::Remove requests.
     *
     * https://www.google.com/m8/feeds/contacts/%1/full/%2
     * @param user User. "default" or user@gmail.com
     * @param contactID ID of contact to remove
     */
    static QUrl removeContactUrl(const QString &user, const QString &contactID);

    /**
     * Returns URL for KGAPI::Request::FetchAll requests.
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look.
     */
    static QUrl fetchAllGroupsUrl(const QString &user);

    /**
     * Returns URL for KGAPI::Request::Fetch requests.
     *
     * @param user User ("default" or user@gmail.com) in whose addressbook to look
     * @param groupId ID of group of which to fetch all data.
     */
    static QUrl fetchGroupUrl(const QString &user, const QString &groupId);

    /**
     * Returns URL for KGAPI::Request::Create requestse.
     *
     * @param user User (user@gmail.com) in whose addressbook to create a group.
     */
    static QUrl createGroupUrl(const QString &user);

    /**
     * Returns URL for KGAPI::Request::Update requests.
     *
     * @param user User (user@gmail.com)) in whose addressbook to look
     * @param groupID ID of group to update.
     */
    static QUrl updateGroupUrl(const QString &user, const QString &groupId);

    /**
     * Returns URL for KGAPI::Request::Remove requests.
     *
     * @param user User (user@gmail.com) in whose addressbook to look
     * @param groupID ID of group to delete
     */
    static QUrl removeGroupUrl(const QString &user, const QString &groupId);

    /**
     * Returns URL for modifying photo. Send KGAPI::Request::Fetch
     * to download the photo, KGAPI::Request::Update to change photo
     * or KGAPI::Request::Remove to remove the photo.
     *
     * @param user User (user@gmail.com) in whose addressbook to lok
     * @param contactId ID of contacts whose photo to update
     */
    static QUrl photoUrl(const QString &user, const QString &contactId);

    /**
     * Returns whether service supports reading data in JSON format.
     *
     * @param urlParam Returns a value of "alt" URL query. Usually is
     * "json" or "jsonc". When service does not support reading JSON
     * data, the value remains unchanged.
     */
    static bool supportsJSONRead(QString* urlParam);

    /**
     * Returns whether service supports writing data in JSON format.
     *
     * @param urlParam Returns a value of "alt" URL query. Usually is
     * "json" or "jsonc". When service does not support writing JSON
     * data, the value remains unchanged.
     */
    static bool supportsJSONWrite(QString* urlParam);

protected:
    ContactsPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(Contacts);

};

} // namespace Services

} // namespace KGAPI

Q_DECLARE_METATYPE(KGAPI::Services::Contacts);

#endif // LIBKGAPI_SERVICES_CONTACTS_H
