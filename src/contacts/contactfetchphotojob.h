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

#ifndef CONTACTFETCHPHOTOJOB_H
#define CONTACTFETCHPHOTOJOB_H

#include "fetchjob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2
{

/**
 * @brief A job to fetch contact photos
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactFetchPhotoJob : public KGAPI2::FetchJob
{
    Q_OBJECT
  public:

    /**
     * @brief Constructs a job that will fetch photos for given @p contacts
     *
     * @param contacts Contacts for which to fetch photos
     * @param account Account to authenticate the requests
     * @param parent
     */
    explicit ContactFetchPhotoJob(const ContactsList &contacts,
                                  const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Constructs a job that will fetch photos for given @p contact
     *
     * @param contact Contact for which to fetch photo
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactFetchPhotoJob(const ContactPtr &contact,
                                  const AccountPtr &account, QObject *parent = 0);

    /**
     * @brief Destructor
     */
    virtual ~ContactFetchPhotoJob();

  Q_SIGNALS:

    /**
     * @brief A signal that is emitted every time a photo is retrieved.
     *
     * Not that there is no way to access fetched photos after the job is
     * finished.
     *
     * @param job This job
     * @param contact A contact with KContacts::Addressee::photo filled
     */
    void photoFetched(KGAPI2::Job *job, const KGAPI2::ContactPtr &contact);

  protected:

    /**
     * @brief KGAPI2::Job::start implementation
     */
    virtual void start();

    /**
     * @brief KGAPI2::Job::handleReply implementation
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

#endif // CONTACTFETCHPHOTOJOB_H
