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

#ifndef LIBKGAPI2_DELETEJOB_H
#define LIBKGAPI2_DELETEJOB_H

#include <libkgapi2/job.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @headerfile DeleteJob
 * @brief Abstract superclass for all jobs that delete resources from Google
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT DeleteJob : public KGAPI2::Job
{
    Q_OBJECT

  public:

    /**
     * Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit DeleteJob(QObject* parent = 0);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests send by this job
     * @param parent
     */
    explicit DeleteJob(const KGAPI2::AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~DeleteJob();

  protected:

    /**
     * @brief KGAPI::Job::dispatchRequest immplementation
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
     * KGAPI2::Job::handleReply implementation
     *
     * Ths implementation will automatically call start() again in order to
     * process another item in queue. Programmer should call emitFinished()
     * from start() if there are no further items to delete.
     *
     * If you need more control over deleting or handling the reply, you can
     * reimplement this method in your subclass.
     *
     * @param rawData
     * @param contentType
     */
    virtual void handleReply(const QNetworkReply *reply, const QByteArray& rawData);

  private:
    class Private;
    Private const * d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_DELETEJOB_H
