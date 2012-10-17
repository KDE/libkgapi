/*
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "staticmap.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <kdebug.h>

namespace KGAPI
{

namespace Services
{

class StaticMapPrivate
{

public:
    StaticMapPrivate()
    {manager = 0;};
    QNetworkAccessManager * manager;
};

}

}

using namespace KGAPI::Objects;
using namespace KGAPI::Services;

StaticMap::StaticMap(QObject * parent) : QObject(parent), d_ptr(new StaticMapPrivate())
{
    Q_D(StaticMap);
    d->manager = new QNetworkAccessManager();

    connect(d->manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

StaticMap::StaticMap (StaticMapPrivate& other) : d_ptr(&other)
{
    Q_D(StaticMap);
    connect(d->manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

StaticMap::~StaticMap()
{
    delete d_ptr;
}

void StaticMap::fetchTile(const QUrl & url)
{
    Q_D(StaticMap);
    d->manager->get(QNetworkRequest(url));
}

void StaticMap::fetchTile (StaticMapUrl& url)
{
    Q_D(StaticMap);
    d->manager->get(QNetworkRequest(url.url()));
}

void StaticMap::replyFinished(QNetworkReply * reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        kWarning() << "Error in" << reply->url() << ":" << reply->errorString();
        return;
    }

    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());

    Q_EMIT tileFetched(pixmap);
}
