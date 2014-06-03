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

#ifndef LIBKGAPI2_CONTACTCREATEJOB_H
#define LIBKGAPI2_CONTACTCREATEJOB_H

#include <libkgapi2/createjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @brief A job to create one or more new contacts in Google Contacts addressbook
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class LIBKGAPI2_EXPORT ContactCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT
  public:

    /**
     * @brief Constructs a job that will create given @p contact in user's
     *        addressbook
     *
     * @param contact Contact to store
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactCreateJob(const ContactPtr &contact, const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Constructs a job that will create given @p contacts in user's
     *        addressbook
     *
     * @param contacts Contacts to store
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactCreateJob(const ContactsList &contacts, const AccountPtr &account, QObject* parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~ContactCreateJob();

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::dispatchRequest implementation
     *
     * @param accessManager
     * @param request
     * @param data
     * @param contentType
     */
    virtual void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType);

    /**
     * @brief KGAPI2::Job::handleReply implementation
     *
     * @param rawData
     * @param contentType
     */
    virtual ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray& rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTCREATEJOB_H
