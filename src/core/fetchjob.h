/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "job.h"
#include "kgapicore_export.h"

namespace KGAPI2 {

/**
 * @headerfile fetchjob.h
 * @brief Abstract superclass for all jobs that fetch resources from Google
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICORE_EXPORT FetchJob : public KGAPI2::Job
{
    Q_OBJECT

  public:

    /**
     * @brief Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit FetchJob(QObject* parent = nullptr);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests sent by this job
     * @param parent
     */
    explicit FetchJob(const KGAPI2::AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FetchJob() override;

    /**
     * @brief Returns all items fetched by this job.
     *
     * Returns all items fetch by this job. This method can be called only
     * from handler of Job::finished signal. Calling this method on a running
     * job will print a warning and return an empty list.
     *
     * @return All items fetched by this job.
     */
    virtual ObjectsList items() const;

  protected:

    /**
     * @brief KGAPI::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request,
                                 const QByteArray& data, const QString& contentType) override;

    /**
     * @brief KGAPI::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply, const QByteArray& rawData) override;

    /**
     * @brief KGAPI::Job::aboutToStart implementation
     */
    void aboutToStart() override;

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

