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


#ifndef LIBKGOOGLE_REPLY_H
#define LIBKGOOGLE_REPLY_H

#include <qnetworkreply.h>

#include <libkgoogle/common.h>
#include <libkgoogle/request.h>
#include <libkgoogle/libkgoogle_export.h>

namespace KGoogle {

  class Object;

  class ReplyPrivate;

  /**
   * Represents a reply received from a Google service.
   */
  class LIBKGOOGLE_EXPORT Reply: public QNetworkReply
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
       */
      Reply(const KGoogle::Request::RequestType requestType, const KGoogle::Error error,
            const QString &serviceName, const QList< KGoogle::Object* > &replyData,
            KGoogle::Request *request, const QByteArray &rawData = QByteArray());

      virtual ~Reply();

      /**
       * Returns type of request this reply relates to.
       */
      KGoogle::Request::RequestType requestType() const;

      /**
       * Returns error code received from Google service
       */
      KGoogle::Error error() const;

      /**
       * Returns name of service this reply came from. Note that
       * the service name relates to a KGoogleService subclasses.
       */
      const QString& serviceName() const;

      /**
       * Returns list of KGoogleObject objects representing data
       * received from the remote service.
       */
      const QList< KGoogle::Object* >& replyData() const;

      /**
       * Returns the original request passed to KGoogleAccessManager.
       */
      KGoogle::Request* request() const;

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
       * once), KGoogle::Reply keeps the data cached, so you can call readAll()
       * multiple times.
       */
      QByteArray readAll() const;

    protected:
      virtual qint64 readData(char *data, qint64 maxSize);

      ReplyPrivate* const d_ptr;

    private:
      Q_DECLARE_PRIVATE(Reply)
  };

} // namespace KGoogle

#endif // LIBKGOOGLE_REPLY_H
