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


#include "fetchlistjob.h"
#include "fetchlistjob_p.h"

#include "kgoogleaccessmanager.h"
#include "kgoogleauth.h"
#include "kgooglerequest.h"
#include "kgooglereply.h"

using namespace KGoogle;

FetchListJob::FetchListJob(const QUrl &url, const QString &service, KGoogleAuth *auth):
  KJob(),
  d_ptr(new FetchListJobPrivate(this, auth))
{
  Q_D(FetchListJob);

  d->auth = auth;
  d->service = service;
  d->url = url;
}

FetchListJob::FetchListJob(const FetchListJob &other):
  KJob(),
  d_ptr(new FetchListJobPrivate(this, d_func()->auth))
{
  Q_D(FetchListJob);

  d->auth = other.auth();
  d->service = other.service();
  d->url = other.url();
}


FetchListJob::~FetchListJob()
{
  delete d_ptr;
}


void FetchListJob::start()
{
  Q_D(FetchListJob);

  d->request = new KGoogleRequest(d->url, KGoogleRequest::FetchAll, d->service);

  d->isRunning = true;
  d->items.clear();
  d->gam->sendRequest(d->request);
}

QList< KGoogleObject* > FetchListJob::items() const
{
  Q_D(const FetchListJob);

  if (!d->isRunning)
    return d->items;

  return QList< KGoogleObject* >();
}

void FetchListJob::setAuth(KGoogleAuth *auth)
{
  Q_D(FetchListJob);

  d->auth = auth;
}

KGoogleAuth *FetchListJob::auth() const
{
  Q_D(const FetchListJob);

  return d->auth;
}



void FetchListJob::setUrl(const QUrl &url)
{
  Q_D(FetchListJob);

  d->url = url;
}

QUrl FetchListJob::url() const
{
  Q_D(const FetchListJob);

  return d->url;
}

void FetchListJob::setService(const QString &service)
{
  Q_D(FetchListJob);

  d->service = service;
}

QString FetchListJob::service() const
{
  Q_D(const FetchListJob);

  return d->service;
}