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
class KGAPILATITUDE_EXPORT LocationCreateJob : public KGAPI2::CreateJob
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
                               const AccountPtr &account, QObject* parent = Q_NULLPTR);

    /**
     * @brief Destructor
     */
    virtual ~LocationCreateJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::CreateJob::handleReplyWithItems implementation
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

#endif // LIBKGAPI2_LOCATIONCREATEJOB_H
