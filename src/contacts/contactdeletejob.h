/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2
{

/**
 * @brief A job to delete one or more contacts from addressbook in
 *        user's Google Contacts account
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new job that will delete given @p contact from
     *        user's addressbook
     *
     * @param contact Contact to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const ContactPtr &contact, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete given @p contacts from
     *        user's addressbook
     *
     * @param contacts Contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const ContactsList &contacts, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete contact with given
     *        @p contactId from user's addressbook
     *
     * @param contactId ID of contact to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QString &contactId, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete contacts with given
     *        @p contactsIds from user's addressbook
     *
     * @param contactIds IDs of contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QStringList &contactIds, const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    ~ContactDeleteJob() override;

protected:
    /**
     * @brief KGAPI::Job::start implementation
     */
    void start() override;

    /**
     * @brief KGAPI::Job::handleReply implementation
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
