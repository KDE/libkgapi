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

#ifndef LIBKGAPI_FETCHLISTJOB_P_H
#define LIBKGAPI_FETCHLISTJOB_P_H

#include <QtCore/QObject>

#include "fetchlistjob.h"

#include "accessmanager.h"
#include "reply.h"
#include "auth.h"

namespace KGAPI
{

/**
 * \internal
 */
class FetchListJobPrivate: public QObject
{
    Q_OBJECT

  public:
    explicit FetchListJobPrivate(FetchListJob* const parent):
        QObject(0),
        request(0),
        q_ptr(parent)
    {
        gam = new AccessManager();
        connect(gam, SIGNAL(replyReceived(KGAPI::Reply*)),
                this, SLOT(replyReceived(KGAPI::Reply*)));
        connect(gam, SIGNAL(requestFinished(KGAPI::Request*)),
                this, SLOT(requestFinished(KGAPI::Request*)));
        connect(gam, SIGNAL(error(KGAPI::Error, QString)),
                this, SLOT(error(KGAPI::Error, QString)));
        connect(gam, SIGNAL(requestProgress(KGAPI::Request*, int, int)),
                this, SLOT(gamRequestProgress(KGAPI::Request*, int, int)));

        isRunning = false;
    }

    virtual ~FetchListJobPrivate()
    {
        if (gam)
            delete gam;

        if (request)
            delete request;
    }

    KGAPI::Request *request;
    KGAPI::AccessManager *gam;
    QList< KGAPI::Object* > items;

    QUrl url;
    KGAPI::Request::RequestType requestType;
    QString service;
    QString accountName;

    bool isRunning;

  private Q_SLOTS:
    void replyReceived(KGAPI::Reply *reply)
    {
        items << reply->replyData();

        delete reply;
    }

    void requestFinished(KGAPI::Request *request)
    {
        Q_Q(FetchListJob);

        isRunning = false;

        q->emitResult();

        Q_UNUSED(request);
    }

    void error(KGAPI::Error errCode, const QString &msg)
    {
        Q_Q(FetchListJob);

        if (errCode == KGAPI::OK)
            return;

        q->setError(errCode);
        q->setErrorText(msg);

        isRunning = false;

        Q_EMIT q->emitResult();
    }

    void gamRequestProgress(KGAPI::Request *request, int processed, int total)
    {
        Q_Q(FetchListJob);

        q->emitPercent(processed, total);

        Q_UNUSED(request);
    }

  private:
    KGAPI::FetchListJob * const q_ptr;
    Q_DECLARE_PUBLIC(FetchListJob);

};

}

#endif /* LIBKAPI_FETCHLISTJOB_P_H */
