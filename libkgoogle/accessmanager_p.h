/*
    libKGoogle - AccessManager
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

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

#ifndef LIBKGOOGLE_ACCESSMANAGER_P_H_
#define LIBKGOOGLE_ACCESSMANAGER_P_H_

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QSemaphore>
#include <QtNetwork/QNetworkReply>

#include <KIO/AccessManager>

namespace KGoogle
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

    QQueue< KGoogle::Request* > cache;
    QSemaphore *cacheSemaphore;;

  public Q_SLOTS:
    void nam_replyReceived(QNetworkReply *reply);
    void nam_sendRequest(KGoogle::Request *request);

    void authenticated();

    void submitCache();

  private:
    AccessManager* const q_ptr;
    Q_DECLARE_PUBLIC(AccessManager)
};

}

#endif /* LIBKGOOGLE_ACCESSMANAGER_P_H_ */