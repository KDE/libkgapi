/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapicore_export.h"

namespace KGAPI2 {

/**
 * @brief A job to fetch AccountInfo
 *
 * AccountInfo provides basic information about user authenticated by given
 * Account.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since: 2.0
 */
class KGAPICORE_EXPORT AccountInfoFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a new AccountInfoFetchJob
     *
     * @param account Account for which to retrieve the info
     * @param parent
     */
    explicit AccountInfoFetchJob(const AccountPtr &account, QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~AccountInfoFetchJob() override;

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
                                             const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

