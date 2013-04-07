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

#ifndef LIBKGAPI_REPLY_H
#define LIBKGAPI_REPLY_H

#include <QtNetwork/QNetworkReply>

#include <libkgapi/common.h>
#include <libkgapi/request.h>
#include <libkgapi/libkgapi_export.h>

namespace KGAPI
{

class Object;

class ReplyPrivate;

/**
 * Represents a reply received from a Google service.
 */
class LIBKGAPI_EXPORT_DEPRECATED Reply: public QNetworkReply
{
  public:
    /**
     * Constructs a new reply.
     *
     * @param requestType Type of request this reply relates to.
     * @param error Error code received from remote service.
     * @param serviceName Name of service this reply relates to.
     * @param replyData List of objects contained in this reply
     * @param request Original request this is a reply to
     * @param rawData Raw content of the reply
     */
    Reply(const KGAPI::Request::RequestType requestType, const KGAPI::Error error,
          const QString &serviceName, const QList< KGAPI::Object* > &replyData,
          KGAPI::Request *request, const QByteArray &rawData = QByteArray());

    virtual ~Reply();

    /**
     * Returns type of request this reply relates to.
     */
    KGAPI::Request::RequestType requestType() const;

    /**
     * Returns error code received from Google service
     */
    KGAPI::Error error() const;

    /**
     * Returns name of service this reply came from. Note that
     * the service name relates to a KGAPI::Service subclasses.
     */
    QString serviceName() const;

    /**
     * Returns list of KGAPI::Object objects representing data
     * received from the remote service.
     */
    QList< KGAPI::Object* > replyData() const;

    /**
     * Returns the original request passed to KGAPI::AccessManager.
     */
    KGAPI::Request* request() const;

    /**
     * Abort the request this reply belongs to.
     *
     * See QNetworkReply::abort() for more details.
     */
    virtual void abort();

    /**
     * This is an overloaded method.
     *
     * While QNetworkReply acts as a sequential device (data can be read only
     * once), KGAPI::Reply keeps the data cached, so you can call readAll()
     * multiple times.
     */
    QByteArray readAll() const;

  protected:
    virtual qint64 readData(char *data, qint64 maxSize);

  private:
    ReplyPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Reply)
};

} // namespace KGAPI

#endif // LIBKGAPI_REPLY_H
