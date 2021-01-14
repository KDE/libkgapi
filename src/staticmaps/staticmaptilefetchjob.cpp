/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "staticmaptilefetchjob.h"
#include "staticmapurl.h"
#include "../debug.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

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
    Q_UNUSED(data)
    Q_UNUSED(contentType)

    accessManager->get(request);
}

void StaticMapTileFetchJob::handleReply(const QNetworkReply *reply,
                                        const QByteArray& rawData)
{
    Q_UNUSED(reply)

    d->tilePixmap.loadFromData(rawData);
    emitFinished();
}



