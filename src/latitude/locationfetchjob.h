/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_LOCATIONFETCHJOB_H
#define LIBKGAPI2_LOCATIONFETCHJOB_H

#include "fetchjob.h"
#include "latitude.h"
#include "kgapilatitude_export.h"

namespace KGAPI2 {

/**
 * @brief A job to fetch current or a specific locations from user's account on
 *        Google Latitude
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPILATITUDE_DEPRECATED_EXPORT LocationFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * @brief Granularity of the results
     *
     * Specifies how precise the results should be. By default, Latitude::City
     * granularity is used.
     *
     * This property can be modified only when the job is not running.
     *
     * @see setGranularity, granularity
     */
    Q_PROPERTY(Latitude::Granularity granularity READ granularity WRITE setGranularity)

  public:

    /**
     * @brief Constructs a job that will fetch users current locations from
     *        Google Latitude service
     *
     * @param account Account to authenticate the requests
     * @param parent
     */
    explicit LocationFetchJob(const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will fetch specific past user's location from
     *        Google Latitude service
     *
     * @param timestamp Timestamp of the location to fetch
     * @param account Account to authenticate the requests
     * @param parent
     */
    explicit LocationFetchJob(qlonglong timestamp, const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LocationFetchJob() override;

    /**
     * @brief Sets granularity of the requested result. By default it's City.
     *
     * @param granularity
     */
    void setGranularity(Latitude::Granularity granularity);

    /**
     * @brief Returns granularity of the requested location.
     */
    Latitude::Granularity granularity() const;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::FetchJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_LOCATIONFETCHJOB_H
