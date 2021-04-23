/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2 {

/**
 * @brief A job to fetch contacts groups from user's addressbook on Google Contacts
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactsGroupFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will fetch all groups from user's
     *        addressbook
     *
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupFetchJob(const AccountPtr &account,
                                   QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will fetch a group with given @p groupId
     *        from user's addressbook
     *
     * @param groupId ID of group to fetch
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupFetchJob(const QString &groupId,
                                   const AccountPtr &account,
                                   QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ContactsGroupFetchJob() override;

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
                                             const QByteArray& rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

