/*
    libKGoogle - KGoogleRequest
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


#ifndef KGOOGLEREQUEST_H
#define KGOOGLEREQUEST_H

#include "libkgoogle_export.h"

#include <qobject.h>
#include <qlist.h>
#include <qurl.h>
#include <qnetworkrequest.h>

namespace KGoogle {
  class KGoogleObject;
}

namespace KGoogle {
  
  class LIBKGOOGLE_EXPORT KGoogleRequest: public QNetworkRequest
  {

    public:
      enum RequestType { FetchAll, Fetch, Create, Update, Remove };
      
      KGoogleRequest() { };
      KGoogleRequest(const QUrl &url, const RequestType requestType, const QString &serviceName);

      void setRequestType(const RequestType requestType);
      RequestType requestType();
      
      bool setServiceName(const QString &serviceName);
      QString serviceName();
      
      void setRequestData(const QByteArray &object, const QString &contentType);
      QByteArray requestData(QString *contentType = 0);
      QString contentType();
      
      
      void setProperty(const QString &name, const QVariant &value);
      QVariant property(const QString &name);
      
    private:
      RequestType m_requestType;
      QString m_serviceName;
      QByteArray m_requestData;
      QString m_contentType;
      QMap<QString,QVariant> m_properties;

  };

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::KGoogleRequest)
Q_DECLARE_METATYPE(KGoogle::KGoogleRequest*)
  
#endif // KGOOGLEREQUEST_H
