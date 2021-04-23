/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

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
    ~StaticMapTileFetchJob() override;

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

