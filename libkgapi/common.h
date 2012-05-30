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

#ifndef LIBKGAPI_COMMON_H
#define LIBKGAPI_COMMON_H

#include <stdexcept>

#include <qstring.h>
#include <qurl.h>

#include <klocalizedstring.h>

namespace KGAPI
{

/**
 * Error codes for asynchronous operations. When an error occurs, a signal
 * with one of following error codes is emitted.
 */
enum Error {
    /* Internal LibKGAPI errors */
    UnknownError = 1,        /// LibKGAPI error - a general unidentified error.
    AuthError = 2,           /// LibKGAPI error - authentication process failed.
    UnknownAccount = 3,      /// LibKGAPI error - application requested unknown account.
    UnknownService = 4,      /// LibKGAPI error - application requested unknown service.
    InvalidResponse = 5,     /// LibKGAPI error - Google returned invalid response.
    BackendNotReady = 6,     /// LibKGAPI error - backend is not ready (for example KWallet is not opened).
    InvalidAccount = 7,      /// LibKGAPI error - the KGAPI::Account object is invalid.
    NetworkError = 8,        /// LibKGAPI error - standard network request returned other code then 200.
    AuthCancelled = 9,       /// LibKGAPI error - when authentication dialog is canceled

    /* Following error codes identify Google errors */
    OK = 200,                /// Request successfully executed.
    Created = 201,           /// Create request successfully executed.
    NoContent = 204,         /// Tasks API returns 204 when task is successfully removed.
    TemporarilyMoved = 302,  /// The object is located on a different URL provided in reply.
    NotModified = 304,       /// Request was successful, but no data were updated.
    BadRequest = 400,        /// Invalid (malformed) request.
    Unauthorized = 401,      /// Invalid or expired token. See KGAPI::Account::refreshTokens().
    Forbidden = 403,         /// The requested data are not accessible to this account
    NotFound = 404,          /// Requested object was not found on the remote side
    Conflict = 409,          /// Object on the remote site differs from the submitted one. See KGAPI::Object::setEtag().
    Gone = 410,              /// The requested does not exist anymore on the remote site
    InternalError = 500,     /// An unexpected error on the Google service occurred
    QuotaExceeded = 503      /// User quota has been exceeded, the request should be send again later.
};

namespace Exception
{

/**
* Base class for LibKGAPI exceptions.
*
* Exceptions are used for synchronous operations. Errors in
* asynchronous operations are reported via signal with KGAPI::Error code.
*
* Exceptions are available only for libkgapi errors. Errors returned by Google
* service will be always notified about asynchronously via error() signal.
*/
class BaseException: public std::runtime_error
{
public:
    BaseException(const QString &what):
        std::runtime_error(what.toUtf8().constData()) { };
    /**
    * Returns KGAPI::Error code of the exception.
    *
    * @return Returns code of the exception.
    */
    virtual KGAPI::Error code() const = 0;
};

/**
* @see KGAPI::Error::UnknownError
*/
class UnknownError: public BaseException
{
  public:
    UnknownError():
        BaseException(i18n("An unknown error has occurred.")) { };
    KGAPI::Error code() const {
        return KGAPI::UnknownError;
    };
};

/**
* @see KGAPI::Error::AuthError
*/
class AuthError: public BaseException
{
  public:
    AuthError(const QString &what = QString()):
        BaseException(what) { };
    KGAPI::Error code() const {
        return KGAPI::AuthError;
    };
};

/**
* @see KGAPI::Error::UnknownAccount
*/
class UnknownAccount: public BaseException
{
  public:
    UnknownAccount(const QString &accName = QString()):
        BaseException(i18n("Unknown account '%1'", accName)) { };
    KGAPI::Error code() const {
        return KGAPI::UnknownAccount;
    };
};

/**
* @see KGAPI::Error::UnknownService
*/
class UnknownService: public BaseException
{
  public:
    UnknownService(const QString &serviceName = QString()):
        BaseException(i18n("Unknown service '%1'", serviceName)) { };
    KGAPI::Error code() const {
        return KGAPI::UnknownService;
    };
};

/**
* @see KGAPI::Error::InvalidResponse
*/
class InvalidResponse: public BaseException
{
  public:
    InvalidResponse(const QString &what = QString()):
        BaseException(what) { };
    KGAPI::Error code() const {
        return KGAPI::InvalidResponse;
    };
};

/**
* @see KGAPI::Error::BackendNotReady
*/
class BackendNotReady: public BaseException
{
  public:
    BackendNotReady():
        BaseException(i18n("KWallet is not opened.")) { };
    KGAPI::Error code() const {
        return KGAPI::BackendNotReady;
    };
};

/**
* @see KGAPI::Error::InvalidAccount
*/
class InvalidAccount: public BaseException
{
  public:
    InvalidAccount():
        BaseException(i18n("The account is invalid.")) { };
    KGAPI::Error code() const {
        return KGAPI::InvalidAccount;
    };
};

} /* namespace Exception */

/**
 * Struct to store additional information about a feed.
 */
class FeedData {

  public:
    FeedData():
        startIndex(0),
        itemsPerPage(0),
        totalResults(0)
    { };

    int startIndex;     /// Index of first item on current feed page.
    int itemsPerPage;   /// Number of items per feed page. This will be same  for all pages (except for the last one which can be shorter).
    int totalResults;   /// Number of all items.
    QUrl nextPageUrl;   /// Link to next page of feed.
    QUrl requestUrl;    /// Original URL of the request. This value is filled by AccessManager when passing the structure to a service
};


} /* namespace KGAPI */

#endif // LIBKGAPI_COMMON_H
