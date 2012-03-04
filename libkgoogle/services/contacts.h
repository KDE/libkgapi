/*
    libKGoogle - Services - Contacts
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


#ifndef LIBKGOOGLE_SERVICES_CONTACTS_H
#define LIBKGOOGLE_SERVICES_CONTACTS_H

#include <libkgoogle/service.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qmetatype.h>
#include <qdom.h>

namespace KGoogle
{

  class Object;

  namespace Services
  {

    class ContactsPrivate;

    /**
     * Represents Google Contacts service.
     */
    class LIBKGOOGLE_EXPORT Contacts: public KGoogle::Service
    {
      public:
        static QUrl ScopeUrl;

        /**
         * Implementation of KGoogle::Service::JSONToObject().
         */
        KGoogle::Object* JSONToObject (const QByteArray& jsonData);

        /**
         * Implementation of KGoogle::Service::objectToJSON().
         */
        QByteArray objectToJSON (KGoogle::Object* object);

        /**
         * Implementation of KGoogle::Service::parseJSONFeed().
         */
        QList< KGoogle::Object* > parseJSONFeed (const QByteArray& jsonFeed, FeedData* feedData = 0);

        /**
         * Implementation of KGoogle::Service::XMLToObject().
         */
        KGoogle::Object* XMLToObject (const QByteArray& xmlData);

        /**
         * Implementation of KGoogle::Service::objectToXML().
         */
        QByteArray objectToXML (KGoogle::Object* object);

        /**
         * Implementation of KGoogle::Service::parseXMLFeed().
         */
        QList< KGoogle::Object* > parseXMLFeed (const QByteArray& xmlFeed, FeedData* feedData = 0);

        /**
         * Implementation of KGoogle::Service::protocolVersion().
         */
        QString protocolVersion() const;

        /**
         * Returns scope URL for Google Calendar service.
         *
         * https://www.google.com/m8/feeds/
         */
        const QUrl& scopeUrl() const;

        /**
         * Returns URL for KGoogle::Request::Create requests.
         *
         * @param user User. "default" or user@gmail.com}
         */
        static QUrl createContactUrl (const QString &user);

        /**
         * Returns URL for KGoogle::Request::FetchAll requests.
         *
         * @param user User. "default" or user@gmail.com
         * @param showDeleted Whether to fetch deleted contacts placeholders as well
         */
        static QUrl fetchAllContactsUrl (const QString &user, const bool &showDeleted);

        /**
         * Returns URL for KGoogle::Request::Fetch requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full/%2
         * @param user User. "default" or user@gmail.com
         * @param contactID ID of contact to fetch
         */
        static QUrl fetchContactUrl (const QString &user, const QString &contactID);

        /**
         * Returns URL for KGoogle::Request::Update requests.
         *
         * @param user User. "default" or user@gmail.com
         * @param contactID ID of contact to update
         */
        static QUrl updateContactUrl (const QString &user, const QString &contactID);

        /**
         * Returns URL for KGoogle::Request::Remove requests.
         *
         * https://www.google.com/m8/feeds/contacts/%1/full/%2
         * @param user User. "default" or user@gmail.com
         * @param contactID ID of contact to remove
         */
        static QUrl removeContactUrl (const QString &user, const QString &contactID);

        /**
         * Returns URL for KGoogle::Request::FetchAll requests.
         *
         * @param user User ("default" or user@gmail.com) in whose addressbook to look.
         */
        static QUrl fetchAllGroupsUrl (const QString &user);

        /**
         * Returns URL for KGoogle::Request::Fetch requests.
         *
         * @param user User ("default" or user@gmail.com) in whose addressbook to look
         * @param groupId ID of group of which to fetch all data.
         */
        static QUrl fetchGroupUrl (const QString &user, const QString &groupId);

        /**
         * Returns URL for KGoogle::Request::Create requestse.
         *
         * @param user User (user@gmail.com) in whose addressbook to create a group.
         */
        static QUrl createGroupUrl (const QString &user);

        /**
         * Returns URL for KGoogle::Request::Update requests.
         *
         * @param user User (user@gmail.com)) in whose addressbook to look
         * @param groupID ID of group to update.
         */
        static QUrl updateGroupUrl (const QString &user, const QString &groupId);

        /**
         * Returns URL for KGoogle::Request::Remove requests.
         *
         * @param user User (user@gmail.com) in whose addressbook to look
         * @param groupID ID of group to delete
         */
        static QUrl removeGroupUrl (const QString &user, const QString &groupId);

        /**
         * Returns URL for modifying photo. Send KGoogle::Request::Fetch
         * to download the photo, KGoogle::Request::Update to change photo
         * or KGoogle::Request::Remove to remove the photo.
         *
         * @param user User (user@gmail.com) in whose addressbook to lok
         * @param contactId ID of contacts whose photo to update
         */
        static QUrl photoUrl (const QString &user, const QString &contactId);

        /**
         * Returns wheter service supports reading data in JSON format.
         *
         * @param urlParam Returns a value of "alt" URL query. Usually is
         * "json" or "jsonc". When service does not support reading JSON
         * data, the value remains unchanged.
         */
        static bool supportsJSONRead (QString* urlParam);

        /**
         * Returns wheter service supports writing data in JSON format.
         *
         * @param urlParam Returns a value of "alt" URL query. Usually is
         * "json" or "jsonc". When service does not support writing JSON
         * data, the value remains unchanged.
         */
        static bool supportsJSONWrite (QString* urlParam);

      protected:
        ContactsPrivate *d_ptr;

      private:
        Q_DECLARE_PRIVATE(Contacts);

    };

  } // namespace Services

} // namespace KGoogle

Q_DECLARE_METATYPE (KGoogle::Services::Contacts)

#endif // LIBKGOOGLE_SERVICES_CONTACTS_H
