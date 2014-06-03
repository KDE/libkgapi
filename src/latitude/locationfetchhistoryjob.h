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

#ifndef LIBKGAPI2_LOCATIONFETCHHISTORYJOB_H
#define LIBKGAPI2_LOCATIONFETCHHISTORYJOB_H

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>
#include <libkgapi2/latitude/latitudeservice.h>

namespace KGAPI2 {

/**
 * @brief A job to fetch all past locations from user's account on Google Latitude
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT LocationFetchHistoryJob : public KGAPI2::FetchJob
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

    /**
     * @brief Maximum number of results to fetch
     *
     * Specifies up to how many locations should the job fetch. Default value is
     * 0, which means that the job will fetch all available locations.
     *
     * This property can be modified only when the job is not running.
     *
     * @see setMaxResults, maxResults
     */
    Q_PROPERTY(int maxResults READ maxResults WRITE setMaxResults)

    /**
     * @brief Oldest location to fetch
     *
     * Specifies timestamp of recording of the oldest location to be fetched.
     *
     * This property can be modified only when the job is not running.
     *
     * @see minTimestamp, setMinTimestamp
     */
    Q_PROPERTY(qlonglong minTimestamp READ minTimestamp WRITE setMinTimestamp)

    /**
     * @brief Newest location to fetch
     *
     * Specifies timestamp of recording of the newest location to be fetched.
     *
     * This property can be modified only when the job is not running.
     *
     * @see maxTimestamp, setMaxTimestamp
     */
    Q_PROPERTY(qlonglong maxTimestamp READ maxTimestamp WRITE setMaxTimestamp)

  public:

    /**
     * @brief Constructs a job that will fetch all past user's locations from
     *        Google Latitude service
     *
     * @param account Account to authenticate the requests
     * @param parent
     */
    explicit LocationFetchHistoryJob(const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~LocationFetchHistoryJob();

    /**
     * @brief Returns granularity of the requested locations.
     */
    Latitude::Granularity granularity() const;

    /**
     * @brief Sets granularity of the requested results. By default it's City.
     *
     * @param granularity
     */
    void setGranularity(Latitude::Granularity granularity);

    /**
     * @brief Returns maximum number of locations the job will fetch
     */
    int maxResults() const;

    /**
     * @brief Sets maximum number of locations to fetch
     *
     * @param results Maximum number of results or 0 to fetch all locations.
     */
    void setMaxResults(int results);

    /**
     * @brief Returns lower date limit for fetched locations.
     */
    qlonglong minTimestamp() const;

    /**
     * @brief Sets lower date limit for locations to fetch.
     *
     * @param minTimestamp
     */
    void setMinTimestamp(qlonglong minTimestamp);

    /**
     * @brief Returns upper date limit for fetched locations.
     */
    qlonglong maxTimestamp() const;

    /**
     * @brief Sets upper date limit for locations to fetch.
     *
     * @param maxTimestamp
     */
    void setMaxTimestamp(qlonglong maxTimestamp);

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::handleReplyWithItems
     *
     * @param reply
     * @param rawData
     */
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_LOCATIONFETCHHISTORYJOB_H
