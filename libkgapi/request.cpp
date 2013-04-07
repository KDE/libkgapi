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

#include "request.h"
#include "account.h"
#include "debug.h"

#include <qglobal.h>

namespace KGAPI
{

/**
 * \internal
 */
class RequestPrivate
{

  public:
    RequestPrivate() { }

    Request::RequestType requestType;
    QString serviceName;
    QByteArray requestData;
    QString contentType;
    QMap< QString, QVariant > properties;
    KGAPI::Account::Ptr account;

    /* Internal request URL. KGAPI::AccessManager actually uses this url
     * for the requests instead of not QNetworkRequest::url().
     *
     * This allows users to see the original URL (the URL they set) at all times,
     * and AccessManager uses this to track "progress" in multi-paged requests.
     */
    QUrl _requestUrl;

};

}

using namespace KGAPI;

Request::Request():
    QNetworkRequest(),
    d_ptr(new RequestPrivate)
{

}

Request::Request(const QUrl &url, const KGAPI::Request::RequestType requestType,
                 const QString &serviceName, const KGAPI::Account::Ptr &account):
    QNetworkRequest(url),
    d_ptr(new RequestPrivate)
{
    Q_D(Request);

    d->requestType = requestType;
    d->serviceName = serviceName;
    d->account = account;

    d->_requestUrl = url;
}

Request::~Request()
{
    delete d_ptr;
}


void Request::setRequestType(const KGAPI::Request::RequestType requestType)
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
        KGAPIWarning() << "Trying to use unregistered service " << serviceName;
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

    if (contentType) {
        *contentType = d_func()->contentType;
    }

    return d_func()->requestData;
}

const QString& Request::contentType() const
{
    return d_func()->contentType;
}

KGAPI::Account::Ptr Request::account() const
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

void Request::setUrl(const QUrl &url)
{
    Q_D(Request);

    /* Reset the internall process-tracking URL */
    d->_requestUrl = url;

    QNetworkRequest::setUrl(url);
}

/*== Private methods ==*/
QUrl Request::realUrl() const
{
    return d_func()->_requestUrl;
}

void Request::setRealUrl(const QUrl &url)
{
    Q_D(Request);

    d->_requestUrl = url;
}

