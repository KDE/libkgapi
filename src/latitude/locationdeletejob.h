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

#ifndef LIBKGAPI2_LOCATIONDELETEJOB_H
#define LIBKGAPI2_LOCATIONDELETEJOB_H

#include "deletejob.h"
#include "kgapilatitude_export.h"

namespace KGAPI2 {

/**
 * @brief A job to delete one or more location from user's account on
 *        Google Latitude service
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPILATITUDE_EXPORT LocationDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will delete user's current location from
     *        his/her Google Latitude account.
     *
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationDeleteJob(const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a job that will delete given @p location from user's
     *        Google Latitude account.
     *
     * @param location Location to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationDeleteJob(const LocationPtr &location,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a job that will delete given location with given
     *         @p timestamp from user'sGoogle Latitude account.
     *
     * @param timestamp Timestamp of location to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationDeleteJob(qulonglong timestamp,
                               const AccountPtr &account, QObject* parent);

    /**
     * @brief Destructor
     */
    virtual ~LocationDeleteJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_LOCATIONDELETEJOB_H
