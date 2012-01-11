/*
    libKGoogle - Reply
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

#include "reply.h"

#include <qmetatype.h>
#include <qglobal.h>

#include <kdebug.h>

using namespace KGoogle;

KGoogle::Reply::Reply(const KGoogle::Request::RequestType requestType, const KGoogle::Error error,
                      const QString &serviceName, const QList< KGoogle::Object* > &replyData,
                      KGoogle::Request *request, const QByteArray &rawData):
  m_requestType(requestType),
  m_errorCode(error),
  m_replyData(replyData),
  m_request(request),
  m_rawData(rawData)
{
  if (QMetaType::type(qPrintable(serviceName)))
    m_serviceName = serviceName;
}

KGoogle::Reply::~Reply()
{ }

KGoogle::Request::RequestType KGoogle::Reply::requestType()
{
  return m_requestType;
}

KGoogle::Error KGoogle::Reply::error() const
{
  return m_errorCode;
}

const QString& KGoogle::Reply::serviceName() const
{
  return m_serviceName;
}

const QList< KGoogle::Object* >& KGoogle::Reply::replyData() const
{
  return m_replyData;
}

KGoogle::Request* KGoogle::Reply::request() const
{
  return m_request;
}

qint64 KGoogle::Reply::readData(char* data, qint64 maxSize)
{
  return read(data, maxSize);
}

void KGoogle::Reply::abort()
{
  QNetworkReply::close();
}

QByteArray Reply::readAll()
{
  return m_rawData;
}
