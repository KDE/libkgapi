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
    explicit ContactFetchPhotoJob(const ContactsList &contacts, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Constructs a job that will fetch photos for given @p contact
     *
     * @param contact Contact for which to fetch photo
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactFetchPhotoJob(const ContactPtr &contact, const AccountPtr &account, QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ContactFetchPhotoJob() override;

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
