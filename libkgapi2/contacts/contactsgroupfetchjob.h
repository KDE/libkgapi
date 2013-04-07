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

#ifndef LIBKGAPI2_CONTACTSGROUPFETCHJOB_H
#define LIBKGAPI2_CONTACTSGROUPFETCHJOB_H

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to fetch contacts groups from user's addressbook on Google Contacts
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT ContactsGroupFetchJob : public KGAPI2::FetchJob
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
                                   QObject* parent = 0);

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
                                   QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~ContactsGroupFetchJob();

  protected:

      /**
       * @brief KGAPI2::Job::start implementation
       */
    virtual void start();

    /**
     * @brief KGAPI2::FetchJob::handleReplyWithItems implementation
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

#endif // LIBKGAPI2_CONTACTSGROUPFETCHJOB_H
