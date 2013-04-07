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
#include "debug.h"

#include <libkgapi2/staticmaps/staticmaptilefetchjob.h>

namespace KGAPI
{

namespace Services
{

class StaticMap::Private
{
  public:
    Private(StaticMap *parent);
    void _k_fetchTileFinished(KGAPI2::Job *job);

  private:
    StaticMap *q;
};

}

}

using namespace KGAPI::Objects;
using namespace KGAPI::Services;

StaticMap::Private::Private(StaticMap* parent):
    q(parent)
{
}

void StaticMap::Private::_k_fetchTileFinished(KGAPI2::Job* job)
{
    KGAPI2::StaticMapTileFetchJob *fetchJob = qobject_cast<KGAPI2::StaticMapTileFetchJob*>(job);

    Q_EMIT q->tileFetched(fetchJob->tilePixmap());
}

StaticMap::StaticMap(QObject * parent):
    QObject(parent),
    d(new Private(this))
{
}

StaticMap::~StaticMap()
{
    delete d;;
}

void StaticMap::fetchTile(const QUrl & url)
{
    KGAPI2::StaticMapTileFetchJob *job = new KGAPI2::StaticMapTileFetchJob(url, this);
    connect(job, SIGNAL(finished(KGAPI2::Job*)),
            this, SLOT(_k_fetchTileFinished(KGAPI2::Job*)));
}

void StaticMap::fetchTile (StaticMapUrl& url)
{
    fetchTile(url.url());
}

#include "staticmap.moc"
