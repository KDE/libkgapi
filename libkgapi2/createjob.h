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

#ifndef LIBKGAPI2_CREATEJOB_H
#define LIBKGAPI2_CREATEJOB_H

#include <libkgapi2/job.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @headerfile CreateJob
 * @brief Abstract superclass for all jobs that create a new objects on the
 *        server.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT CreateJob: public KGAPI2::Job
{
    Q_OBJECT

  public:

    /**
     * @brief Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit CreateJob(QObject* parent = 0);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests send by this job
     * @param parent
     */
    explicit CreateJob(const KGAPI2::AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~CreateJob();

    /**
     * @return Returns newly created items
     */
    virtual ObjectsList items() const;

  protected:

    /**
     * @brief KGAPI::Job::dispatchRequest()
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    virtual void dispatchRequest(QNetworkAccessManager* accessManager,
                                 const QNetworkRequest& request,
                                 const QByteArray& data, const QString& contentType);

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * \param rawData
     * \param contentType
     */
    virtual void handleReply(const QNetworkReply *reply, const QByteArray& rawData);

    /**
     * @brief KGAPI2::Job::aboutToStart() implementation
     */
    virtual void aboutToStart();

    /**
     * @brief A reply handler that returns items parsed from \@ rawData
     *
     * This method can be reimplemented in a FetchJob subclasses. It is called
     * automatically when a reply is received and the returned items are stored
     * in FetchJob and accessible via FetchJob::items when the job has finished.
     *
     * If you need more control over handling reply and items, you can reimplement
     * FetchJob::handleReply. Note that reimplementing FetchJob::handleReply
     * usually requires reimplementing FetchJob::items as well and storing the
     * parsed items in your implementation.
     *
     * @param reply A QNetworkReply received from Google server
     * @param rawData Content of body of the @p reply. Don't use
     *        QNetworkReply::readAll(), because the content has already been read
     *        by Job implementation and thus it would return empty data.
     *
     * @return Items parsed from @p rawData
     */
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray &rawData);


  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CREATEJOB_H
