/*
    LibkGoogle - FetchListJob
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

#ifndef LIBKGOOGLE_FETCHLISTJOB_P_H
#define LIBKGOOGLE_FETCHLISTJOB_P_H

#include <QtCore/QObject>

#include "fetchlistjob.h"

#include "accessmanager.h"
#include "reply.h"
#include "auth.h"

namespace KGoogle
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
        q_ptr(parent)
    {
        gam = new AccessManager();
        connect(gam, SIGNAL(replyReceived(KGoogle::Reply*)),
                this, SLOT(replyReceived(KGoogle::Reply*)));
        connect(gam, SIGNAL(requestFinished(KGoogle::Request*)),
                this, SLOT(requestFinished(KGoogle::Request*)));
        connect(gam, SIGNAL(error(KGoogle::Error, QString)),
                this, SLOT(error(KGoogle::Error, QString)));
        connect(gam, SIGNAL(requestProgress(KGoogle::Request*, int, int)),
                this, SLOT(gamRequestProgress(KGoogle::Request*, int, int)));

        isRunning = false;
    }

    virtual ~FetchListJobPrivate()
    {
        delete gam;
        delete request;
    }

    KGoogle::Request *request;
    KGoogle::AccessManager *gam;
    QList< KGoogle::Object* > items;

    QUrl url;
    KGoogle::Request::RequestType requestType;
    QString service;
    QString accountName;

    bool isRunning;

  private Q_SLOTS:
    void replyReceived(KGoogle::Reply *reply)
    {
        items << reply->replyData();

        delete reply;
    }

    void requestFinished(KGoogle::Request *request)
    {
        Q_Q(FetchListJob);

        isRunning = false;

        q->emitResult();

        Q_UNUSED(request);
    }

    void error(KGoogle::Error errCode, const QString &msg)
    {
        Q_Q(FetchListJob);

        if (errCode == KGoogle::OK)
            return;

        q->setError(errCode);
        q->setErrorText(msg);
    }

    void gamRequestProgress(KGoogle::Request *request, int processed, int total)
    {
        Q_Q(FetchListJob);

        q->emitPercent(processed, total);

        Q_UNUSED(request);
    }

  private:
    KGoogle::FetchListJob * const q_ptr;
    Q_DECLARE_PUBLIC(FetchListJob);

};

}

#endif /* LIBKGOOGLE_FETCHLISTJOB_P_H */
