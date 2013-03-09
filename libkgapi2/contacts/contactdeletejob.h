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

#ifndef LIBKGAPI2_CONTACTDELETEJOB_H
#define LIBKGAPI2_CONTACTDELETEJOB_H

#include <libkgapi2/deletejob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to delete one or more contacts from addressbook in
 *        user's Google Contacts account
 *
 * Note that operation is irreversible.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT ContactDeleteJob : public KGAPI2::DeleteJob
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
    explicit ContactDeleteJob(const ContactPtr &contact,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete given @p contacts from
     *        user's addressbook
     *
     * @param contacts Contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const ContactsList &contacts,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete contact with given
     *        @p contactId from user's addressbook
     *
     * @param contactId ID of contact to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QString &contactId,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Constructs a new job that will delete contacts with given
     *        @p contactsIds from user's addressbook
     *
     * @param contactsIds IDs of contacts to delete
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactDeleteJob(const QStringList contactIds,
                              const AccountPtr &account, QObject* parent);

    /**
     * @brief Destructor
     */
    virtual ~ContactDeleteJob();

  protected:

    /**
     * @brief KGAPI::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI::Job::handleReply implementation
     *
     * @param reply
     * @param rawData
     */
    virtual void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTDELETEJOB_H
