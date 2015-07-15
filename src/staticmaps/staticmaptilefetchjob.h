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

#ifndef LIBKGAPI2_STATICMAPTILEFETCHJOB_H
#define LIBKGAPI2_STATICMAPTILEFETCHJOB_H

#include "job.h"
#include "kgapimaps_export.h"

#include <QtGui/QPixmap>

/**
 * @brief A job to fetch a single map tile described by a StaticMapUrl
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
namespace KGAPI2 {

class StaticMapUrl;

class KGAPIMAPS_EXPORT StaticMapTileFetchJob : public KGAPI2::Job
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will fetch a map tile described by given
     *        @p url.
     *
     * @param url
     * @param parent
     */
    explicit StaticMapTileFetchJob(const StaticMapUrl &url, QObject* parent = Q_NULLPTR);

    /**
     * @overload StaticMapTileFetchJob(StaticMapUrl,QObject*)
     *
     * @param url
     * @param parent
     */
    explicit StaticMapTileFetchJob(const QUrl &url, QObject* parent = Q_NULLPTR);

    /**
     * @brief Destructor
     */
    virtual ~StaticMapTileFetchJob();

    /**
     * @brief Pixmap with the fetched map tile.
     *
     * This method can only be called after the job has emitted Job::finished.
     */
    QPixmap tilePixmap() const;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    virtual void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data,
                                 const QString& contentType);

    /**
     * @brief KGAPI2::Job::handleReply
     *
     * @param reply
     * @param rawData
     */
    virtual void handleReply(const QNetworkReply *reply,
                             const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_STATICMAPTILEFETCHJOB_H
