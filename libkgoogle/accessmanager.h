/*
    libKGoogle - AccessManager
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


#ifndef LIBKGOOGLE_ACCESSMANAGER_H
#define LIBKGOOGLE_ACCESSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include <KDE/KDateTime>

#include <libkgoogle/common.h>
#include <libkgoogle/libkgoogle_export.h>

class QNetworkReply;

namespace KGoogle
{
class Auth;
class Reply;
class Request;
}

namespace KGoogle
{

class AccessManagerPrivate;

/**
 * AccessManager allows application to send and receive requests from
 * Google services.
 */
class LIBKGOOGLE_EXPORT AccessManager: public QObject
{

    Q_OBJECT

  public:
    /**
     * Constructs a new access manager
     */
    AccessManager();

    virtual ~AccessManager();

    /**
     * Converts RFC3339 date and time to KDateTime object.
     *
     * KDateTime::fromString() supportes RFC3339 date
     * since KDE 4.7.0. This function uses native implementation
     * when available and provides custom implementation for
     * backwards compatibilityt with older KDE versions.
     *
     * @param datetime String with date and time in RFC3339 format
     * @return \p datetime converted to KDateTime object
     */
    static KDateTime RFC3339StringToDate(const QString &datetime);

    /**
     * Converts KDateTime object do RFC3339 string.
     *
     * KDateTime::toString() supportes RFC3339 date
     * since KDE 4.7.0. This function uses native implementation
     * when available and provides custom implementation for
     * backwards compatibilityt with older KDE versions.
     *
     * @param datetime KDateTime object to be converted
     * @return \p datetime converted to RFC3339 string
     */
    static QString dateToRFC3339String(const KDateTime &datetime);


  public Q_SLOTS:
    /**
     * Sends a \p request to a Google service.
     *
     * When a reply is received, the replyReceived() signal
     * is emitted. When Google sends more replies (usually
     * when the amount of items is more then 25 Google splits
     * the reply to multiple smaller replies) the requestFinished()
     * signal is emitted after the last reply is received.
     *
     * @param request A request to be send
     */
    void sendRequest(KGoogle::Request *request);


  Q_SIGNALS:
    /**
     * A reply was received.
     *
     * This signal is emitted whenever a reply from
     * a Google service was received.
     */
    void replyReceived(KGoogle::Reply *reply);

    /**
     * A request was finished.
     *
     * This signal is emitted when last batch of data is received,
     * for example when all pages of events feed are fetched.
     *
     * @param request The related request.
     */
    void requestFinished(KGoogle::Request *request);


    /**
     * Informs about progress of retrieving reply to \p request.
     *
     * When reply to the \p request is split to multiple parts,
     * this signal is emitted whenever a new chunk of data is received.
     *
     * When the reply consists of only one part, this signal is not emitted.
     *
     * @param request A KGoogle::Request to which the data belong
     * @param processed Amount of received items
     * @param totalItems Total amount of items to be received
     */
    void requestProgress(KGoogle::Request *request, int processed, int totalItems);

    /**
     * An error occurred.
     *
     * This signal is emitted whenever an error
     * occurs.
     */
    void error(const KGoogle::Error errCode, const QString &msg);

  private:
    AccessManagerPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(AccessManager)

};

}  // namespace KGoogle

#endif // LIBKGOOGLEA_CCESSMANAGER_H
