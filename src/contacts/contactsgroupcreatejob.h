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

#ifndef LIBKGAPI2_CONTACTSGROUPCREATEJOB_H
#define LIBKGAPI2_CONTACTSGROUPCREATEJOB_H

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
                                    QObject* parent = Q_NULLPTR);

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
                                    QObject* parent = Q_NULLPTR);

    /**
     * @brief Destructor
     */
    virtual ~ContactsGroupCreateJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    void start() Q_DECL_OVERRIDE;

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param rawData
     * @param contentType
     */
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTSGROUPCREATEJOB_H
