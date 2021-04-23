/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "createjob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2 {

/**
 * @brief A job to create one or more new groups (folders) in Google Contacts 
 *       addressbook
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactsGroupCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:

    /**
     * @brief Constructs a job that will create given @p groups in user's
     *        addressbook
     *
     * @param groups Groups to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupCreateJob(const ContactsGroupsList &groups,
                                    const AccountPtr &account,
                                    QObject* parent = nullptr);

    /**
     * @brief Constructs a job that will create given @p group in user's
     *        addressbook
     *
     * @param group Group to create
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupCreateJob(const ContactsGroupPtr &group,
                                    const AccountPtr &account,
                                    QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ContactsGroupCreateJob() override;

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI2::Job::handleReply implementation
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

