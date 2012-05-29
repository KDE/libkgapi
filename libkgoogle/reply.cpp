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

namespace KGoogle
{

/**
 * \internal
 */
class ReplyPrivate
{

  public:
    KGoogle::Request::RequestType requestType;
    KGoogle::Error errorCode;
    QString serviceName;
    QList< KGoogle::Object* > replyData;
    KGoogle::Request *request;
    FeedData *feedData;

    QByteArray rawData;
};

}

using namespace KGoogle;

KGoogle::Reply::Reply(const KGoogle::Request::RequestType requestType, const KGoogle::Error error,
                      const QString &serviceName, const QList< KGoogle::Object* > &replyData,
                      KGoogle::Request *request, const QByteArray &rawData):
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

KGoogle::Reply::~Reply()
{ }

KGoogle::Request::RequestType KGoogle::Reply::requestType() const
{
    return d_func()->requestType;
}

KGoogle::Error KGoogle::Reply::error() const
{
    return d_func()->errorCode;
}

const QString& KGoogle::Reply::serviceName() const
{
    return d_func()->serviceName;
}

const QList< KGoogle::Object* >& KGoogle::Reply::replyData() const
{
    return d_func()->replyData;
}

KGoogle::Request* KGoogle::Reply::request() const
{
    return d_func()->request;
}

qint64 KGoogle::Reply::readData(char* data, qint64 maxSize)
{
    return read(data, maxSize);
}

void KGoogle::Reply::abort()
{
    QNetworkReply::close();
}

QByteArray Reply::readAll() const
{
    return d_func()->rawData;
}
