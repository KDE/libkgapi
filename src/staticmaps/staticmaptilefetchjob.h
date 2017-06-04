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

#ifndef LIBKGAPI2_STATICMAPTILEFETCHJOB_H
#define LIBKGAPI2_STATICMAPTILEFETCHJOB_H

#include "job.h"
#include "kgapimaps_export.h"

#include <QPixmap>

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
    explicit StaticMapTileFetchJob(const StaticMapUrl &url, QObject* parent = nullptr);

    /**
     * @overload StaticMapTileFetchJob(StaticMapUrl,QObject*)
     *
     * @param url
     * @param parent
     */
    explicit StaticMapTileFetchJob(const QUrl &url, QObject* parent = nullptr);

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
    void start() override;

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data,
                                 const QString& contentType) override;

    /**
     * @brief KGAPI2::Job::handleReply
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply,
                             const QByteArray& rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_STATICMAPTILEFETCHJOB_H
