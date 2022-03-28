/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapilatitude_export.h"

namespace KGAPI2
{

/**
 * @brief A job to delete one or more location from user's account on
 *        Google Latitude service
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPILATITUDE_DEPRECATED_EXPORT LocationDeleteJob : public KGAPI2::DeleteJob
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
    explicit LocationDeleteJob(const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a job that will delete given @p location from user's
     *        Google Latitude account.
     *
     * @param location Location to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationDeleteJob(const LocationPtr &location, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a job that will delete given location with given
     *         @p timestamp from user'sGoogle Latitude account.
     *
     * @param timestamp Timestamp of location to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit LocationDeleteJob(qulonglong timestamp, const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    ~LocationDeleteJob() override;

protected:
    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2
