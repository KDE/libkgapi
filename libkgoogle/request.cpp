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

namespace KGoogle {

  /**
   * \internal
   */
  class RequestPrivate {

    public:
      RequestPrivate() { }

      Request::RequestType requestType;
      QString serviceName;
      QByteArray requestData;
      QString contentType;
      QMap< QString,QVariant > properties;
      KGoogle::Account *account;
  };

}

using namespace KGoogle;

Request::Request():
  QNetworkRequest(),
  d_ptr(new RequestPrivate)
{

}

Request::Request(const QUrl &url, const KGoogle::Request::RequestType requestType,
                 const QString &serviceName, KGoogle::Account *account):
  QNetworkRequest(url),
  d_ptr(new RequestPrivate)
{
  Q_D(Request);

  d->requestType = requestType;
  d->serviceName = serviceName;
  d->account = account;
}

Request::~Request()
{
  delete d_ptr;
}


void Request::setRequestType(const KGoogle::Request::RequestType requestType)
{
  Q_D(Request);

  d->requestType = requestType;
}

Request::RequestType Request::requestType() const
{
  return d_func()->requestType;
}

bool Request::setServiceName(const QString& serviceName)
{
  Q_D(Request);

  if (QMetaType::type(qPrintable(serviceName)) == 0) {
    kWarning() << "Trying to use unregistered service " << serviceName;
    return false;
  }

  d->serviceName = serviceName;

  return true;
}

const QString& Request::serviceName() const
{
  return d_func()->serviceName;
}

void Request::setRequestData(const QByteArray& data, const QString &contentType)
{
  Q_D(Request);

  d->requestData = data;
  d->contentType = contentType;
}

const QByteArray& Request::requestData(QString *contentType) const
{

  if (contentType)
    *contentType = d_func()->contentType;

  return d_func()->requestData;
}

const QString& Request::contentType() const
{
  return d_func()->contentType;
}

KGoogle::Account* Request::account() const
{
  return d_func()->account;
}

void Request::setProperty(const QString& name, const QVariant& value)
{
  Q_D(Request);
  d->properties.insert(name, value);
}

QVariant Request::property(const QString& name) const
{
  return d_func()->properties.value(name);
}

bool Request::hasProperty(const QString& name) const
{
  return d_func()->properties.contains(name);
}
