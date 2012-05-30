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

#ifndef LIBKGAPI_ACCESSMANAGER_P_H
#define LIBKGAPI_ACCESSMANAGER_P_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QSemaphore>
#include <QtNetwork/QNetworkReply>

#include <KIO/AccessManager>

namespace KGAPI
{

class AccessManager;

class Request;

/**
 * \internal
 */
class AccessManagerPrivate: public QObject
{

    Q_OBJECT

  public:
    AccessManagerPrivate(AccessManager* const parent);

    virtual ~AccessManagerPrivate();

    KIO::Integration::AccessManager *nam;

    QQueue< KGAPI::Request* > cache;
    QSemaphore *cacheSemaphore;;

  public Q_SLOTS:
    void nam_replyReceived(QNetworkReply *reply);
    void nam_sendRequest(KGAPI::Request *request);

    void authenticated();

    void submitCache();

  private:
    QString parseErrorMessage(const QByteArray &json) const;

    AccessManager* const q_ptr;
    Q_DECLARE_PUBLIC(AccessManager)
};

}

#endif /* LIBKGAPI_ACCESSMANAGER_P_H */
