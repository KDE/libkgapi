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

#ifndef LIBKGAPI2_TYPES_H
#define LIBKGAPI2_TYPES_H

#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QList>
#include <QtCore/QUrl>

namespace KGAPI2
{

/**
 * @brief Structure to store additional information about a feed.
 */
class LIBKGAPI2_EXPORT FeedData {

  public:
    explicit FeedData():
        startIndex(0),
        itemsPerPage(0),
        totalResults(0)
    { };

    int startIndex;     /**< Index of first item on current feed page. */
    int itemsPerPage;   /**< Number of items per feed page. This will be same 
                             for all pages (except for the last one which can be
                             shorter). */
    int totalResults;   /**< Number of all items. */
    QUrl nextPageUrl;   /**< Link to next page of feed.*/
    QUrl requestUrl;    /**< Original URL of the request. This value is filled 
                             by AccessManager when passing the structure to a
                             service */
};


class Object;
typedef QSharedPointer<Object> ObjectPtr;
typedef QList<ObjectPtr> ObjectsList;

class Account;
typedef QSharedPointer<Account> AccountPtr;
typedef QList<AccountPtr> AccountsList;

class AccountInfo;
typedef QSharedPointer<AccountInfo> AccountInfoPtr;
typedef QList<AccountInfoPtr> AccountInfosList;

class Contact;
typedef QSharedPointer<Contact> ContactPtr;
typedef QList<ContactPtr> ContactsList;

class ContactsGroup;
typedef QSharedPointer<ContactsGroup> ContactsGroupPtr;
typedef QList<ContactsGroupPtr> ContactsGroupsList;

class Calendar;
typedef QSharedPointer<Calendar> CalendarPtr;
typedef QList<CalendarPtr> CalendarsList;

namespace Drive
{

class About;
typedef QSharedPointer<About> AboutPtr;
typedef QList<AboutPtr> AboutsList;

class App;
typedef QSharedPointer<App> AppPtr;
typedef QList<AppPtr> AppsList;

class Change;
typedef QSharedPointer<Change> ChangePtr;
typedef QList<ChangePtr> ChangesList;

class ChildReference;
typedef QSharedPointer<ChildReference> ChildReferencePtr;
typedef QList<ChildReferencePtr> ChildReferencesList;

class File;
typedef QSharedPointer<File> FilePtr;
typedef QList<FilePtr> FilesList;

class ParentReference;
typedef QSharedPointer<ParentReference> ParentReferencePtr;
typedef QList<ParentReferencePtr> ParentReferencesList;

class Permission;
typedef QSharedPointer<Permission> PermissionPtr;
typedef QList<PermissionPtr> PermissionsList;

class Revision;
typedef QSharedPointer<Revision> RevisionPtr;
typedef QList<RevisionPtr> RevisionsList;

class User;
typedef QSharedPointer<User> UserPtr;
typedef QList<UserPtr> UsersList;

}

class Event;
typedef QSharedPointer<Event> EventPtr;
typedef QList<EventPtr> EventsList;

class Location;
typedef QSharedPointer<Location> LocationPtr;
typedef QList<LocationPtr> LocationsList;

class Reminder;
typedef QSharedPointer<Reminder> ReminderPtr;
typedef QList<ReminderPtr> RemindersList;

class Task;
typedef QSharedPointer<Task> TaskPtr;
typedef QList<TaskPtr> TasksList;

class TaskList;
typedef QSharedPointer<TaskList> TaskListPtr;
typedef QList<TaskListPtr> TaskListsList;

template<class T>
ObjectsList operator<<(ObjectsList &objectsList, const QList< QSharedPointer<T> > &list)
{
    Q_FOREACH (const QSharedPointer<T> &item, list) {
        objectsList << item;
    }

    return objectsList;
}

/**
 * @brief Job error codes
 */
enum Error {
    /* Internal LibKGAPI errors */
    NoError = 0,             ///< LibKGAPI error - no error.
    UnknownError = 1,        ///< LibKGAPI error - a general unidentified error.
    AuthError = 2,           ///< LibKGAPI error - authentication process failed.
    UnknownAccount = 3,      ///< LibKGAPI error - application requested unknown account.
    UnknownService = 4,      ///< LibKGAPI error - application requested unknown service.
    InvalidResponse = 5,     ///< LibKGAPI error - Google returned invalid response.
    BackendNotReady = 6,     ///< @deprecated LibKGAPI error - backend is not ready (for example KWallet is not opened).
    InvalidAccount = 7,      ///< LibKGAPI error - the KGAPI2::Account object is invalid.
    NetworkError = 8,        ///< LibKGAPI error - standard network request returned other code then 200.
    AuthCancelled = 9,       ///< LibKGAPI error - when authentication dialog is canceled

    /* Following error codes identify Google errors */
    OK = 200,                ///< Request successfully executed.
    Created = 201,           ///< Create request successfully executed.
    NoContent = 204,         ///< Tasks API returns 204 when task is successfully removed.
    TemporarilyMoved = 302,  ///< The object is located on a different URL provided in reply.
    NotModified = 304,       ///< Request was successful, but no data were updated.
    BadRequest = 400,        ///< Invalid (malformed) request.
    Unauthorized = 401,      ///< Invalid or expired token. See KGAPI2::Account::refreshTokens().
    Forbidden = 403,         ///< The requested data are not accessible to this account
    NotFound = 404,          ///< Requested object was not found on the remote side
    Conflict = 409,          ///< Object on the remote site differs from the submitted one. @see KGAPI2::Object::setEtag.
    Gone = 410,              ///< The requested does not exist anymore on the remote site
    InternalError = 500,     ///< An unexpected error on the Google service occurred
    QuotaExceeded = 503      ///< User quota has been exceeded, the request should be send again later.
};

/**
 * @since 2.0
 */
enum ContentType {
    UnknownContentType = -1,
    JSON,
    XML
};

} // namespace KGAPI2



#endif // LIBKGAPI2_TYPES_H
