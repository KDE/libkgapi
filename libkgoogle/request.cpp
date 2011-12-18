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


#include "account.h"
#include "request.h"

#include <kdebug.h>
#include <qglobal.h>

using namespace KGoogle;

KGoogle::Request::Request(const QUrl &url,
                          const KGoogle::Request::RequestType requestType,
                          const QString &serviceName, KGoogle::Account *account):
  QNetworkRequest(url),
  m_requestType(requestType),
  m_serviceName(serviceName),
  m_account(account)
{ }

void KGoogle::Request::setRequestType(const KGoogle::Request::RequestType requestType)
{
  m_requestType = requestType;
}

KGoogle::Request::RequestType KGoogle::Request::requestType() const
{
  return m_requestType;
}

bool KGoogle::Request::setServiceName(const QString& serviceName)
{
  if (QMetaType::type(qPrintable(serviceName)) == 0) {
    kWarning() << "Trying to use unregistered service " << serviceName;
    return false;
  }

  m_serviceName = serviceName;
  return true;
}

const QString& KGoogle::Request::serviceName() const
{
  return m_serviceName;
}

void KGoogle::Request::setRequestData(const QByteArray& data, const QString &contentType)
{
  m_requestData = data;
  m_contentType = contentType;
}

const QByteArray& KGoogle::Request::requestData(QString *contentType) const
{
  if (contentType)
    *contentType = m_contentType;

  return m_requestData;
}

const QString& KGoogle::Request::contentType() const
{
  return m_contentType;
}

KGoogle::Account* KGoogle::Request::account() const
{
  return m_account;
}

void KGoogle::Request::setProperty(const QString& name, const QVariant& value)
{
  m_properties.insert(name, value);
}

QVariant KGoogle::Request::property(const QString& name) const
{
  return m_properties.value(name);
}

bool KGoogle::Request::hasProperty(const QString& name) const
{
  return m_properties.contains(name);
}
