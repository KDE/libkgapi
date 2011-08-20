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

#include "libkgoogle_export.h"

#include "kgooglerequest.h"
#include "kgoogleobject.h"

namespace KGoogle {
  
  class LIBKGOOGLE_EXPORT KGoogleReply: public QNetworkReply
  {
    public:
      enum ErrorCode { OK = 200,
		       Created = 201,
		       TemporarilyMoved = 302,
		       NotModified = 304,
		       BadRequest = 400,
		       Unauthorized = 401,
		       Forbidden = 403,
		       NotFound = 404,
		       Conflict = 409,
		       Gone = 410,
		       InternalError = 500 };
      
      KGoogleReply(const KGoogle::KGoogleRequest::RequestType requestType, const ErrorCode error, 
		   const QString &serviceName, const QList<KGoogleObject*> &replyData,
		   KGoogleRequest *request);

      ~KGoogleReply();
      
      KGoogle::KGoogleRequest::RequestType requestType();
      
      ErrorCode error();
      
      QString serviceName();
      
      QList<KGoogleObject*> replyData();
      
      KGoogleRequest* request();
      
      virtual void abort();
      
    protected:
      virtual qint64 readData(char *data, qint64 maxSize);
      
    private:
      KGoogle::KGoogleRequest::RequestType m_requestType;
      ErrorCode m_errorCode;
      QString m_serviceName;
      QList<KGoogleObject*> m_replyData;
      KGoogleRequest *m_request;     
    
  };
  
} // namespace KGoogle

#endif // KGOOGLEREPLY_H
