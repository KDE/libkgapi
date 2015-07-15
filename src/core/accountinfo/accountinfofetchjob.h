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

#ifndef LIBKGAPI2_ACCOUNTINFOFETCHJOB_H
#define LIBKGAPI2_ACCOUNTINFOFETCHJOB_H

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
    explicit AccountInfoFetchJob(const AccountPtr &account, QObject* parent = Q_NULLPTR);

    /**
     * @brief Destructor
     */
    virtual ~AccountInfoFetchJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() Q_DECL_OVERRIDE;

    /**
     * @brief KGAPI2::FetchJob::handleReplyWithItems implementation
     *
     * @param reply
     * @param rawData
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray &rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_ACCOUNTINFOFETCHJOB_H
