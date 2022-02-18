/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapicore_export.h"

#include <QList>
#include <QSharedPointer>
#include <QUrl>

namespace KGAPI2
{

/**
 * @brief Structure to store additional information about a feed.
 */
class KGAPICORE_EXPORT FeedData
{
public:
    explicit FeedData()
        : startIndex(0)
        , itemsPerPage(0)
        , totalResults(0)
    {
    }

    int startIndex; /**< Index of first item on current feed page. */
    int itemsPerPage; /**< Number of items per feed page. This will be same
                           for all pages (except for the last one which can be
                           shorter). */
    int totalResults; /**< Number of all items. */
    QUrl nextPageUrl; /**< Link to next page of feed.*/
    QUrl requestUrl; /**< Original URL of the request. This value is filled
                          by AccessManager when passing the structure to a
                          service */
    QString syncToken; /**< Sync token that can be used for incremental
                            updates by some of the services.*/
};

class Object;
using ObjectPtr = QSharedPointer<Object>;
using ObjectsList = QList<ObjectPtr>;

class Account;
using AccountPtr = QSharedPointer<Account>;
using AccountsList = QList<AccountPtr>;

class AccountInfo;
using AccountInfoPtr = QSharedPointer<AccountInfo>;
using AccountInfosList = QList<AccountInfoPtr>;

namespace People {

class Person;
using PersonPtr = QSharedPointer<Person>;
using PersonList = QList<PersonPtr>;

class ContactGroup;
using ContactGroupPtr = QSharedPointer<ContactGroup>;
using ContactGroupList = QList<ContactGroupPtr>;

}

class Calendar;
using CalendarPtr = QSharedPointer<Calendar>;
using CalendarsList = QList<CalendarPtr>;

namespace Drive
{

class About;
using AboutPtr = QSharedPointer<About>;
using AboutsList = QList<AboutPtr>;

class App;
using AppPtr = QSharedPointer<App>;
using AppsList = QList<AppPtr>;

class Change;
using ChangePtr = QSharedPointer<Change>;
using ChangesList = QList<ChangePtr>;

class ChildReference;
using ChildReferencePtr = QSharedPointer<ChildReference>;
using ChildReferencesList = QList<ChildReferencePtr>;

class File;
using FilePtr = QSharedPointer<File>;
using FilesList = QList<FilePtr>;

class ParentReference;
using ParentReferencePtr = QSharedPointer<ParentReference>;
using ParentReferencesList = QList<ParentReferencePtr>;

class Permission;
using PermissionPtr = QSharedPointer<Permission>;
using PermissionsList = QList<PermissionPtr>;

class Revision;
using RevisionPtr = QSharedPointer<Revision>;
using RevisionsList = QList<RevisionPtr>;

class Drives;
using DrivesPtr = QSharedPointer<Drives>;
using DrivesList = QList<DrivesPtr>;

class Teamdrive;
using TeamdrivePtr = QSharedPointer<Teamdrive>;
using TeamdrivesList = QList<TeamdrivePtr>;

class User;
using UserPtr = QSharedPointer<User>;
using UsersList = QList<UserPtr>;

}

class Event;
using EventPtr = QSharedPointer<Event>;
using EventsList = QList<EventPtr>;

class Location;
using LocationPtr = QSharedPointer<Location>;
using LocationsList = QList<LocationPtr>;

class Reminder;
using ReminderPtr = QSharedPointer<Reminder>;
using RemindersList = QList<ReminderPtr>;

class Task;
using TaskPtr = QSharedPointer<Task>;
using TasksList = QList<TaskPtr>;

class TaskList;
using TaskListPtr = QSharedPointer<TaskList>;
using TaskListsList = QList<TaskListPtr>;

namespace Blogger
{
class Blog;
using BlogPtr = QSharedPointer<Blog>;
using BlogsList = QList<BlogPtr>;

class Comment;
using CommentPtr = QSharedPointer<Comment>;
using CommentsList = QList<CommentPtr>;

class Page;
using PagePtr = QSharedPointer<Page>;
using PagesList = QList<PagePtr>;

class Post;
using PostPtr = QSharedPointer<Post>;
using PostsList = QList<PostPtr>;

}

template<class T>
ObjectsList operator<<(ObjectsList &objectsList, const QList<QSharedPointer<T>> &list)
{
    for (const QSharedPointer<T> &item : list) {
        objectsList << item;
    }

    return objectsList;
}

/**
 * @brief Job error codes
 */
enum Error {
    /* Internal LibKGAPI errors */
    NoError = 0, ///< LibKGAPI error - no error.
    UnknownError = 1, ///< LibKGAPI error - a general unidentified error.
    AuthError = 2, ///< LibKGAPI error - authentication process failed.
    UnknownAccount = 3, ///< LibKGAPI error - application requested unknown account.
    UnknownService = 4, ///< LibKGAPI error - application requested unknown service.
    InvalidResponse = 5, ///< LibKGAPI error - Google returned invalid response.
    BackendNotReady = 6, ///< @deprecated LibKGAPI error - backend is not ready (for example KWallet is not opened).
    InvalidAccount = 7, ///< LibKGAPI error - the KGAPI2::Account object is invalid.
    NetworkError = 8, ///< LibKGAPI error - standard network request returned a different code than 200.
    AuthCancelled = 9, ///< LibKGAPI error - when the authentication dialog is canceled.

    /* Following error codes identify Google errors */
    OK = 200, ///< Request successfully executed.
    Created = 201, ///< Create request successfully executed.
    NoContent = 204, ///< Tasks API returns 204 when task is successfully removed.
    ResumeIncomplete = 308, ///< Drive Api returns 308 when accepting a partial file upload
    TemporarilyMoved = 302, ///< The object is located on a different URL provided in reply.
    NotModified = 304, ///< Request was successful, but no data were updated.
    TemporarilyMovedUseSameMethod = 307, ///< The object is located at a different URL provided in the reply. The same request method must be used.
    BadRequest = 400, ///< Invalid (malformed) request.
    Unauthorized = 401, ///< Invalid or expired token. See KGAPI2::Account::refreshTokens().
    Forbidden = 403, ///< The requested data is not accessible to this account.
    NotFound = 404, ///< Requested object was not found on the remote side.
    Conflict = 409, ///< Object on the remote site differs from the submitted one. @see KGAPI2::Object::setEtag.
    Gone = 410, ///< The requested data does not exist anymore on the remote site.
    InternalError = 500, ///< An unexpected error occurred on the Google service.
    QuotaExceeded = 503 ///< User quota has been exceeded, the request should be sent again later.
};

/**
 * @since 2.0
 */
enum ContentType { UnknownContentType = -1, JSON, XML };

} // namespace KGAPI2
