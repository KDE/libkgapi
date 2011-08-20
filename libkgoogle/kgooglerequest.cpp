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


#include "kgooglerequest.h"
#include "kgoogleobject.h"

#include <kdebug.h>
#include <qglobal.h>

using namespace KGoogle;

KGoogleRequest::KGoogleRequest(const QUrl &url, 
			       const KGoogleRequest::RequestType requestType, 
			       const QString &serviceName):
  QNetworkRequest(url),
  m_requestType(requestType),
  m_serviceName(serviceName)
{ }

void KGoogleRequest::setRequestType(const KGoogleRequest::RequestType requestType)
{
  m_requestType = requestType;
}

KGoogleRequest::RequestType KGoogleRequest::requestType()
{
  return m_requestType;
}

bool KGoogleRequest::setServiceName(const QString& serviceName)
{
  if (QMetaType::type(qPrintable(serviceName)) == 0) {
    kWarning() << "Trying to use unregistered service " << serviceName;
    return false;
  }
    
  m_serviceName = serviceName;
  return true;
}

QString KGoogleRequest::serviceName()
{
  return m_serviceName;
}

void KGoogleRequest::setRequestData(const QByteArray& data, const QString &contentType)
{
  m_requestData = data;
  m_contentType = contentType;
}

QByteArray KGoogleRequest::requestData(QString *contentType)
{
  if (contentType)
    *contentType = m_contentType;

  return m_requestData;
}

QString KGoogleRequest::contentType()
{
  return m_contentType;
}


void KGoogleRequest::setProperty(const QString& name, const QVariant& value)
{
  m_properties.insert(name, value);
}

QVariant KGoogleRequest::property(const QString& name)
{
  return m_properties.value(name);
}
