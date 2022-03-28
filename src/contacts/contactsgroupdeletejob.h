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
 * @brief A job to delete one or more contact groups from addressbook in
 *        user's Google Contacts account
 *
 * Note that no contacts are deleted, as all contacts are always stored
 * in "My Contacts" group, which cannot be removed. Contacts are only linked
 * from "My Contacts" groups to any additional group, so when a group is removed,
 * only membership information is modified.
 *
 * This operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactsGroupDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new job that will delete given @p groups from
     *        user's addressbook
     *
     * @param groups Groups to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const ContactsGroupsList &groups, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete given @p group from
     *        user's addressbook
     *
     * @param group Group to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const ContactsGroupPtr &group, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete groups with given
     *        @p groupsIds from user's addressbook
     *
     * @param groupsIds IDs of groups to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const QStringList &groupsIds, const AccountPtr &account, QObject *parent);

    /**
     * @brief Constructs a new job that will delete group with given @p groupsId
     *        from user's addressbook
     *
     * @param groupId ID of group to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const QString &groupId, const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    ~ContactsGroupDeleteJob() override;

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
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2
