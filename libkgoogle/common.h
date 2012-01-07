/*
    libKGoogle - Common
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


#ifndef LIBKGOOGLE_COMMON_H
#define LIBKGOOGLE_COMMON_H

#include <stdexcept>

#include <qstring.h>
#include <qurl.h>

#include <klocalizedstring.h>

namespace KGoogle {


  /**
   * ClientID token for Google API.
   */
  const QString APIClientID = "554041944266.apps.googleusercontent.com";

  /**
   * ClientSecret token for Google API.
   */
  const QString APIClientSecret = "mdT1DjzohxN3npUUzkENT0gO";

  /**
   * Error codes for asynchronous operations. When an error occurs, a signal
   * with one of following error codes is emitted.
   */
  enum Error {
    /* Internal libKGoogle errors */
    UnknownError = 1,        /// libKGoogle error - a general unidentified error.
    AuthError = 2,           /// libKGoogle error - authentication process failed.
    UnknownAccount = 3,      /// libKGoogle error - application requested unknown account.
    UnknownService = 4,      /// libKGoogle error - application requested unknown service.
    InvalidResponse = 5,     /// libKGoogle error - Google returned invalid response.
    BackendNotReady = 6,     /// libKGoogle error - backend is not ready (for example KWallet is not opened).
    InvalidAccount = 7,      /// libKGoogle error - the KGoogle::Account object is invalid.
    NetworkError = 8,        /// libKGoogle error - standard network request returned other code then 200.

    /* Following error codes identify Google errors */
    OK = 200,                /// Request succesfully executed.
    Created = 201,           /// Create request successfuly executed.
    NoContent = 204,         /// Tasks API returns 204 when task is sucessfully removed.
    TemporarilyMoved = 302,  /// The object is located on a different URL provided in reply.
    NotModified = 304,       /// Request was successfull, but no data were updated.
    BadRequest = 400,        /// Invalid (malformed) request.
    Unauthorized = 401,      /// Invalid or expired token. See KGoogleAccount::refreshTokens().
    Forbidden = 403,         /// The requested data are not accessible to this account
    NotFound = 404,          /// Requested object was not found on the remote side
    Conflict = 409,          /// Object on the remote site differs from the submitted one. See KGoogle::Object::setEtag().
    Gone = 410,              /// The requested does not exist anymore on the remote site
    InternalError = 500      /// An unexpected error on the Google service occuerd
  };

  namespace Exception {

    /**
    * Base class for libKGoogle exceptions.
    *
    * Exceptions are used for synchronous operations. Errors in
    * asynchronous operations are reported via signal with KGoogle::Error code.
    *
    * Exceptions are available only for libkgoogle errors. Errors returned by Google
    * service will be always notified about asynchronously via error() signal.
    */
    class BaseException: public std::runtime_error
    {
      public:
        BaseException(const QString &what):
          std::runtime_error(what.toUtf8().constData()) { };
        /**
        * Returns KGoogle::Error code of the exception.
        *
        * @return Returns code of the exception.
        */
        virtual KGoogle::Error code() const = 0;
    };

    /**
    * @see KGoogle::Error::UnknownError
    */
    class UnknownError: public BaseException {
      public:
        UnknownError():
          BaseException(i18n("An unknown error has occured.")) { };
        KGoogle::Error code() const { return KGoogle::UnknownError; };
    };

    /**
    * @see KGoogle::Error::AuthError
    */
    class AuthError: public BaseException {
      public:
        AuthError(const QString &what = QString()):
          BaseException(what) { };
        KGoogle::Error code() const { return KGoogle::AuthError; };
    };

    /**
    * @see KGoogle::Error::UnknownAccount
    */
    class UnknownAccount: public BaseException {
      public:
        UnknownAccount(const QString &accName = QString()):
          BaseException(i18n("Unknown account '%1'", accName)) { };
        KGoogle::Error code() const { return KGoogle::UnknownAccount; };
    };

    /**
    * @see KGoogle::Error::UnknownService
    */
    class UnknownService: public BaseException {
      public:
        UnknownService(const QString &serviceName = QString()):
          BaseException(i18n("Unknown service '%1'", serviceName)) { };
        KGoogle::Error code() const { return KGoogle::UnknownService; };
    };

    /**
    * @see KGoogle::Error::InvalidResponse
    */
    class InvalidResponse: public BaseException {
      public:
        InvalidResponse(const QString &what = QString()):
          BaseException(what) { };
        KGoogle::Error code() const { return KGoogle::InvalidResponse; };
    };

    /**
    * @see KGoogle::Error::BackendNotReady
    */
    class BackendNotReady: public BaseException {
      public:
        BackendNotReady():
          BaseException(i18n("KWallet is not opened.")) { };
        KGoogle::Error code() const { return KGoogle::BackendNotReady; };
    };

    /**
    * @see KGoogle::Error::InvalidAccount
    */
    class InvalidAccount: public BaseException {
      public:
        InvalidAccount():
          BaseException(i18n("The account is invalid.")) { };
        KGoogle::Error code() const { return KGoogle::InvalidAccount; };
    };

  } /* namespace Exception */

  /**
   * Struct to store additional informations about a feed.
   */
  typedef struct FeedData {
    int startIndex;     /// Index of first item on current feed page.
    int itemsPerPage;   /// Number of items per feed page. This will be same  for all pages (except for the last one which can be shorter).
    int totalResults;   /// Number of all items.
    QUrl nextLink;      /// Link to next page of feed.
  } FeedData;


} /* namespace KGoogle */

#endif // LIBKGOOGLE_COMMON_H
