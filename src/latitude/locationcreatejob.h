/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_LOCATIONCREATEJOB_H
#define LIBKGAPI2_LOCATIONCREATEJOB_H

#include "createjob.h"
#include "kgapilatitude_export.h"

namespace KGAPI2 {

/**
 * @brief A job to stores one or more Locations in Google Latitude service
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPILATITUDE_DEPRECATED_EXPORT LocationCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will store given @p location in user's
     *        Google Latitude account
     *
     * @param location Location to store
     * @param isCurrent Whether the location is user's current location
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationCreateJob(const LocationPtr &location, bool isCurrent,
                               const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LocationCreateJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::CreateJob::handleReplyWithItems implementation
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

#endif // LIBKGAPI2_LOCATIONCREATEJOB_H
