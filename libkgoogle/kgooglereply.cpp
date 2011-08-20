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

#include "kgooglereply.h"

#include <qmetatype.h>
#include <qglobal.h>

using namespace KGoogle;

KGoogleReply::KGoogleReply(const KGoogleRequest::RequestType requestType, const KGoogleReply::ErrorCode error, 
			   const QString &serviceName, const QList<KGoogleObject*> &replyData,
			   KGoogleRequest *request):
  m_requestType(requestType),
  m_errorCode(error),
  m_replyData(replyData),
  m_request(request)
{
  if (QMetaType::type(qPrintable(serviceName)))
    m_serviceName = serviceName;
}

KGoogleReply::~KGoogleReply()
{ }

KGoogleRequest::RequestType KGoogleReply::requestType()
{
  return m_requestType;
}

KGoogleReply::ErrorCode KGoogleReply::error()
{
  return m_errorCode;
}

QString KGoogleReply::serviceName()
{
  return m_serviceName;
}

QList<KGoogleObject*> KGoogleReply::replyData()
{
  return m_replyData;
}

KGoogleRequest* KGoogleReply::request()
{
  return m_request;
}

qint64 KGoogleReply::readData(char* data, qint64 maxSize)
{
  return read(data, maxSize);
}

void KGoogleReply::abort()
{
  QNetworkReply::close();
}
