/*
    libKGoogle - Request
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


#ifndef LIBKGOOGLE_REQUEST_H
#define LIBKGOOGLE_REQUEST_H

#include <QtNetwork/QNetworkRequest>
#include <QtCore/QList>
#include <QtCore/QUrl>

#include <libkgoogle/libkgoogle_export.h>
#include <libkgoogle/account.h>

namespace KGoogle
{

class RequestPrivate;

/**
 * Represents a request to be send to a Google service.
 */
class LIBKGOOGLE_EXPORT Request: public QNetworkRequest
{

  public:
    enum RequestType {
        FetchAll,       /** GET **/
        Fetch,          /** GET **/
        Create,         /** POST **/
        Move,           /** POST **/
        Update,         /** PUT **/
        Remove          /** DELETE **/
    };

    /**
     * Constructs an empty request
     */
    Request();

    /**
     * Constructs a new request
     *
     * @param url Url to send the request to.
     * @param requestType Type of request
     * @param serviceName Name of service this request belongs to.
     * @param account Google Account to which the request should be sent
     */
    Request(const QUrl &url, const RequestType requestType, const QString &serviceName, const KGoogle::Account::Ptr &account);


    virtual ~Request();

    /**
     * Sets type of this request.
     */
    void setRequestType(const RequestType requestType);

    /**
     * Returns type of this request.
     */
    RequestType requestType() const;

    /**
     * Sets name of a service.
     *
     * @see KGoogleService
     */
    bool setServiceName(const QString &serviceName);

    /**
     * Returns name of service this requests belongs to
     */
    const QString& serviceName() const;

    /**
     * Sets raw request data and it's type.
     *
     * Data can represent single or multiple objects serialized
     * in XML or JSON format. Note, that not all services accept
     * both, XML and JSON data, some services (like Google Calendar)
     * support JSON for read and XML for write access.
     *
     * @param data Raw data to be send to remote service.
     * @param contentType Type of the data. Should be "application/xml"
     * or "application/json".
     */
    void setRequestData(const QByteArray &data, const QString &contentType);

    /**
     * Returns raw data assigned to the request.
     *
     * @param contentType pointer to a QString [default:NULL].
     */
    const QByteArray& requestData(QString *contentType = 0) const;

    /**
     * Returns mime type of the data
     */
    const QString& contentType() const;

    /**
     * Returns account to which the request is sent.
     */
    KGoogle::Account::Ptr account() const;

    /**
     * Set a value of an objects property.
     *
     * This mimics functionality of QObject::setProperty(). It
     * allows user to store additional informations as object's
     * properties.
     *
     * @param name Name of a property.
     * @param value Value of a property.
     */
    void setProperty(const QString &name, const QVariant &value);

    /**
     * Returns value of a property assigned by setProperty()
     */
    QVariant property(const QString &name) const;

    /**
     * Returns wheter request has a property \p name.
     */
    bool hasProperty(const QString &name) const;

  private:
    RequestPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Request)

};

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Request)
Q_DECLARE_METATYPE(KGoogle::Request*)

#endif // LIBKGOOGLE_REQUEST_H
