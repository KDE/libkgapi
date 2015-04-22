/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "staticmaptilefetchjob.h"
#include "staticmapurl.h"
#include "../debug.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtGui/QPixmap>

using namespace KGAPI2;

class StaticMapTileFetchJob::Private
{
  public:
    QUrl url;
    QPixmap tilePixmap;
};

StaticMapTileFetchJob::StaticMapTileFetchJob(const StaticMapUrl& url,
                                             QObject* parent):
    Job(parent),
    d(new Private)
{
    d->url = url.url();
}

StaticMapTileFetchJob::StaticMapTileFetchJob(const QUrl& url, QObject* parent):
    Job(parent),
    d(new Private)
{
    d->url = url;
}

StaticMapTileFetchJob::~StaticMapTileFetchJob()
{
    delete d;
}

QPixmap StaticMapTileFetchJob::tilePixmap() const
{
    if (isRunning()) {
        qCWarning(KGAPIDebug) << "Called tilePixmap on a running job!";
        return QPixmap();
    }

    return d->tilePixmap;
}

void StaticMapTileFetchJob::start()
{
    enqueueRequest(QNetworkRequest(d->url));
}

void StaticMapTileFetchJob::dispatchRequest(QNetworkAccessManager* accessManager,
                                            const QNetworkRequest& request,
                                            const QByteArray& data,
                                            const QString& contentType)
{
    Q_UNUSED(data);
    Q_UNUSED(contentType);

    accessManager->get(request);
}

void StaticMapTileFetchJob::handleReply(const QNetworkReply *reply,
                                        const QByteArray& rawData)
{
    Q_UNUSED(reply);

    d->tilePixmap.loadFromData(rawData);
    emitFinished();
}



