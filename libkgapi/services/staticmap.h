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


#ifndef LIBKGAPI_SERVICES_STATICMAP_H
#define LIBKGAPI_SERVICES_STATICMAP_H

#include <libkgapi/objects/staticmapurl.h>
#include <libkgapi/service.h>
#include <libkgapi/libkgapi_export.h>

#include <QObject>
#include <QUrl>
#include <QPixmap>

class QNetworkReply;

namespace KGAPI2 {
    class Job;
}

namespace KGAPI
{

namespace Services
{

class LIBKGAPI_EXPORT_DEPRECATED StaticMap : public QObject
{
    Q_OBJECT

  public:
    explicit StaticMap(QObject * parent);

    ~StaticMap();

    void fetchTile(const QUrl & url);

    void fetchTile(KGAPI::Objects::StaticMapUrl &url);


  Q_SIGNALS:
    void tileFetched(const QPixmap & pixmap);

  private:
    class Private;
    Private * const d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_fetchTileFinished(KGAPI2::Job *job))
};

} // namespace Services

} // namespace KGAPI

#endif // LIBKGAPI_SERVICES_STATICMAP_H
