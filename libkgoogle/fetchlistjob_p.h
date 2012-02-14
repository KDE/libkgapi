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

#include <qobject.h>

#include "fetchlistjob.h"

#include "kgoogleaccessmanager.h"
#include "kgooglereply.h"
#include "kgoogleauth.h"

namespace KGoogle {

  class FetchListJobPrivate: public QObject
  {
    Q_OBJECT

  public:
    explicit FetchListJobPrivate(FetchListJob* const parent, KGoogleAuth *auth):
    QObject(0),
    q_ptr(parent)
    {
      gam = new KGoogleAccessManager(auth);
      connect(gam, SIGNAL(replyReceived(KGoogleReply*)),
	      this, SLOT(replyReceived(KGoogleReply*)));
      connect(gam, SIGNAL(requestFinished(KGoogleRequest*)),
	      this, SLOT(requestFinished(KGoogleRequest*)));
      connect(gam, SIGNAL(error(QString,int)),
	      this, SLOT(error(QString,int)));

      isRunning = false;
    }

    virtual ~FetchListJobPrivate()
    {
      delete gam;
      delete request;
    }

    KGoogleRequest *request;
    KGoogleAccessManager *gam;
    QList< KGoogleObject* > items;

    QUrl url;
    KGoogleRequest::RequestType requestType;
    QString service;
    KGoogleAuth *auth;

    bool isRunning;

  private Q_SLOTS:
    void replyReceived(KGoogleReply *reply)
    {
      items << reply->replyData();

      delete reply;
    }

    void requestFinished(KGoogleRequest *request)
    {
      Q_Q(FetchListJob);

      isRunning = false;

      q->emitResult();

      Q_UNUSED(request);
    }

    void error(const QString &err, int code)
    {
      Q_Q(FetchListJob);

      if (code == KGoogleReply::OK)
	return;

      q->setError(code);
      q->setErrorText(err);
    }

  private:
    KGoogle::FetchListJob * const q_ptr;
    Q_DECLARE_PUBLIC(FetchListJob);

  };

}

#endif /* LIBKGOOGLE_FETCHLISTJOB_P_H */
