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

namespace KGAPI2
{

/**
 * @headerfile deletejob.h
 * @brief Abstract superclass for all jobs that delete resources from Google
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICORE_EXPORT DeleteJob : public KGAPI2::Job
{
    Q_OBJECT

public:
    /**
     * Constructor for jobs that don't require authentication
     *
     * @param parent
     */
    explicit DeleteJob(QObject *parent = nullptr);

    /**
     * @brief Constructor for jobs that require authentication
     *
     * @param account Account to use to authenticate the requests sent by this job
     * @param parent
     */
    explicit DeleteJob(const KGAPI2::AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DeleteJob() override;

protected:
    /**
     * @brief KGAPI::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) override;

    /**
     * KGAPI2::Job::handleReply implementation
     *
     * This implementation will automatically call start() again in order to
     * process another item in queue. Programmer should call emitFinished()
     * from start() if there are no further items to delete.
     *
     * If you need more control over deleting or handling the reply, you can
     * reimplement this method in your subclass.
     *
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2
