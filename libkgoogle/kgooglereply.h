/*
    libKGoogle - KGoogleReply
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


#ifndef KGOOGLEREPLY_H
#define KGOOGLEREPLY_H

#include <qnetworkreply.h>

#include <libkgoogle/kgooglerequest.h>
#include <libkgoogle/kgoogleobject.h>
#include <libkgoogle/kgoogleservice.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {
  
  /**
   * Represents a reply received from a Google service.
   */ 
  class LIBKGOOGLE_EXPORT KGoogleReply: public QNetworkReply
  {
    public:
      enum ErrorCode { OK = 200,		/// Request succesfully executed.
		       Created = 201,		/// Create request successfuly executed.
		       NoContent = 204,		/// Tasks API returns 204 when task is sucessfully removed.
		       TemporarilyMoved = 302,	/// The object is located on a different URL provided in reply.
		       NotModified = 304,	/// Request was successfull, but no data were updated.
		       BadRequest = 400,	/// Invalid (malformed) request.
		       Unauthorized = 401,	/// Invalid or expired token. See KGoogleAccount::refreshTokens().
		       Forbidden = 403,		/// The requested data are not accessible to this account
		       NotFound = 404,		/// Requested object was not found on the remote side
		       Conflict = 409,		/// Object on the remote site differs from the submitted one. See KGoogleObject::setEtag().
		       Gone = 410,		/// The requested does not exist anymore on the remote site
		       InternalError = 500 	/// An unexpected error on the Google service occuerd
      };
      
      /**
       * Constructs a new reply.
       * 
       * @param requestType Type of request this reply relates to.
       * @param error Error code received from remote service.
       * @param serviceName Name of service this reply relates to.
       * @param replyData List of objects contained in this reply
       * @param request Original request this is a reply to
       */
      KGoogleReply(const KGoogle::KGoogleRequest::RequestType requestType, const ErrorCode error, 
		   const QString &serviceName, const QList<KGoogleObject*> &replyData,
		   KGoogleRequest *request);

      ~KGoogleReply();
      
      /**
       * Returns type of request this reply relates to.
       */
      KGoogle::KGoogleRequest::RequestType requestType();
      
      /**
       * Returns error code received from Google service
       */
      ErrorCode error();
      
      /**
       * Returns name of service this reply came from. Note that
       * the service name relates to a KGoogleService subclasses.
       */
      QString serviceName();
      
      /**
       * Returns list of KGoogleObject objects representing data
       * received from the remote service.
       */
      QList<KGoogleObject*> replyData();
      
      /**
       * Returns the original request passed to KGoogleAccessManager.
       */
      KGoogleRequest* request();
      
      /**
       * Abort the request this reply belongs to.
       * 
       * See QNetworkReply::abort() for more details.
       */
      virtual void abort();
      
    protected:
      virtual qint64 readData(char *data, qint64 maxSize);
      
    private:
      KGoogle::KGoogleRequest::RequestType m_requestType;
      ErrorCode m_errorCode;
      QString m_serviceName;
      QList<KGoogleObject*> m_replyData;
      KGoogleRequest *m_request;     
      FeedData *m_feedData;
    
  };
  
} // namespace KGoogle

#endif // KGOOGLEREPLY_H
