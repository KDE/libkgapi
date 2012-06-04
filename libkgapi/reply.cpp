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

#include "reply.h"
#include "object.h"

#include <QtCore/QMetaType>
#include <QtCore/qglobal.h>

#include <KDebug>

namespace KGAPI
{

/**
 * \internal
 */
class ReplyPrivate
{

  public:
    KGAPI::Request::RequestType requestType;
    KGAPI::Error errorCode;
    QString serviceName;
    QList< KGAPI::Object* > replyData;
    KGAPI::Request *request;
    FeedData *feedData;

    QByteArray rawData;
};

}

using namespace KGAPI;

KGAPI::Reply::Reply(const KGAPI::Request::RequestType requestType, const KGAPI::Error error,
                      const QString &serviceName, const QList< KGAPI::Object* > &replyData,
                      KGAPI::Request *request, const QByteArray &rawData):
    d_ptr(new ReplyPrivate)
{
    Q_D(Reply);

    d->requestType = requestType;
    d->errorCode = error;
    d->replyData = replyData;
    d->request = request;
    d->rawData = rawData;

    if (QMetaType::type(qPrintable(serviceName)))
        d->serviceName = serviceName;
}

KGAPI::Reply::~Reply()
{
   delete d_ptr;	
}

KGAPI::Request::RequestType KGAPI::Reply::requestType() const
{
    return d_func()->requestType;
}

KGAPI::Error KGAPI::Reply::error() const
{
    return d_func()->errorCode;
}

const QString& KGAPI::Reply::serviceName() const
{
    return d_func()->serviceName;
}

const QList< KGAPI::Object* >& KGAPI::Reply::replyData() const
{
    return d_func()->replyData;
}

KGAPI::Request* KGAPI::Reply::request() const
{
    return d_func()->request;
}

qint64 KGAPI::Reply::readData(char* data, qint64 maxSize)
{
    return read(data, maxSize);
}

void KGAPI::Reply::abort()
{
    QNetworkReply::close();
}

QByteArray Reply::readAll() const
{
    return d_func()->rawData;
}
