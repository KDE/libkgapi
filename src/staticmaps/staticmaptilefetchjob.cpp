/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "staticmaptilefetchjob.h"
#include "staticmapurl.h"
#include "../debug.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtGui/QPixmap>

using namespace KGAPI2;

class Q_DECL_HIDDEN StaticMapTileFetchJob::Private
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



