/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBKGAPI2_CONTACTSGROUPDELETEJOB_H
#define LIBKGAPI2_CONTACTSGROUPDELETEJOB_H

#include "deletejob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2 {

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
    explicit ContactsGroupDeleteJob(const ContactsGroupsList &groups,
                                    const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p group from
     *        user's addressbook
     *
     * @param group Group to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const ContactsGroupPtr &group,
                                    const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete groups with given
     *        @p groupsIds from user's addressbook
     *
     * @param groupsIds IDs of groups to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const QStringList &groupsIds,
                                    const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete group with given @p groupsId
     *        from user's addressbook
     *
     * @param groupId ID of group to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactsGroupDeleteJob(const QString &groupId,
                                    const AccountPtr &account, QObject *parent);

    /**
     * @brief Destructor
     */
    virtual ~ContactsGroupDeleteJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() Q_DECL_OVERRIDE;

    /**
     * @brief KGAPI2::Job::handleReply implementation
     * 
     * @param reply
     * @param rawData
     */
    void handleReply(const QNetworkReply* reply, const QByteArray& rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTSGROUPDELETEJOB_H
