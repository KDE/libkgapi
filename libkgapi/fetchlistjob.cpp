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

#include "fetchlistjob.h"
#include "fetchlistjob_p.h"

#include "accessmanager.h"
#include "request.h"
#include "reply.h"

using namespace KGAPI;

FetchListJob::FetchListJob(const QUrl &url, const QString &service, const QString &accountName):
    KJob(),
    d_ptr(new FetchListJobPrivate(this))
{
    Q_D(FetchListJob);

    d->accountName = accountName;
    d->service = service;
    d->url = url;
}

FetchListJob::FetchListJob(const FetchListJob &other):
    KJob(),
    d_ptr(new FetchListJobPrivate(this))
{
    Q_D(FetchListJob);

    d->accountName = other.accountName();
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

    KGAPI::Auth *auth = KGAPI::Auth::instance();
    KGAPI::Account::Ptr account;
    try {
        account = auth->getAccount(d->accountName);
    } catch (KGAPI::Exception::BaseException &e) {
        setError(e.code());
        setErrorText(QLatin1String(e.what()));
        emitResult();
        return;
    }

    d->request = new Request(d->url, Request::FetchAll, d->service, account);

    d->isRunning = true;
    d->items.clear();
    d->gam->sendRequest(d->request);
}

QList< KGAPI::Object* > FetchListJob::items() const
{
    Q_D(const FetchListJob);

    if (!d->isRunning)
        return d->items;

    return QList< KGAPI::Object* >();
}

void FetchListJob::setAccountName(const QString &accountName)
{
    Q_D(FetchListJob);

    d->accountName = accountName;
}

QString FetchListJob::accountName() const
{
    Q_D(const FetchListJob);

    return d->accountName;
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
