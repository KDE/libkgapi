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

#ifndef LIBKGAPI2_CONTACTFETCHJOB_H
#define LIBKGAPI2_CONTACTFETCHJOB_H

#include "fetchjob.h"
#include "kgapicontacts_export.h"

namespace KGAPI2 {

/**
 * @brief A job to fetch contacts from user's addressbook on Google Contacts
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 2.0
 */
class KGAPICONTACTS_EXPORT ContactFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * @brief Whether to fetch deleted contacts as well
     *
     * When a contact is deleted in Google Contacts, it's stored as a placeholder
     * on Google server and can still be retrieved. Such event will have
     * KGAPI2::Contact::deleted set to @p true.
     *
     * By default, the job will fetch deleted contacts.
     *
     * This property does not have any effect when fetching a specific contact and
     * can be modified only when the job is not running.
     *
     * @see setFetchDeleted, fetchDeleted
     */
    Q_PROPERTY(bool fetchDeleted READ fetchDeleted WRITE setFetchDeleted)

    /**
     * @brief Timestamp to fetch only contacts modified since then
     *
     * When set, this job will only fetch contacts that have been modified since
     * given timestamp.
     *
     * By default the timestamp is 0 and all contacts are fetched.
     *
     * This property does not have any effect when fetching a specific contact and
     * can be modified only when the job is not running.
     *
     * @see setFetchOnlyUpdated, fetchOnlyUpdated
     */
    Q_PROPERTY(quint64 fetchOnlyUpdated READ fetchOnlyUpdated WRITE setFetchOnlyUpdated)


    /**
     * @brief A filter to fetch only contats matching thefulltext search
     *
     * By default the property is empty and no filter is applied.
     *
     * This property does not have any effect when fetching a specific contact and
     * can be modified only when the job is not running.
     *
     * @see setFilter, filter
     */
    Q_PROPERTY(QString filter READ filter WRITE setFilter)

  public:

    /**
     * @brief Constructs a job that will fetch all contacts from user's
     *        addressbook
     *
     * Result of this job might not contain all contacts, depending on configured
     * filters.
     *
     * @param account Account to authenticate the request
     * @param parent
     */
    explicit ContactFetchJob(const AccountPtr &account, QObject* parent = Q_NULLPTR);

    /**
     * @brief Constructs a job that will fetch a contact with given @p contactId
     *        from user's addressbook
     *
     * Note that fetchOnlyUpdated filter is not applied in this case.
     *
     * @param contactId ID of contact to fetch
     * @param account Account to authenticate the request
     * @param parent
     */
    ContactFetchJob(const QString &contactId, const AccountPtr &account, QObject* parent = Q_NULLPTR);

    /**
     * @brief Destructor
     */
    virtual ~ContactFetchJob();

    /**
     * @brief Returns whether deleted contacts are fetched.
     */
    bool fetchDeleted() const;

    /**
     * @brief Sets whether to fetch deleted contacts
     *
     * @param fetchDeleted
     */
    void setFetchDeleted(bool fetchDeleted);

    /**
     * @brief Sets the job to fetch only events modified since @p timestamp
     *
     * @param timestamp
     */
    void setFetchOnlyUpdated(quint64 timestamp);

    /**
     * @brief Returns whether the job will fetch only modified events
     *
     * @return 0 when all events will be fetched, a timestamp of since when the
     *         modified events will be fetched.
     */
    quint64 fetchOnlyUpdated();

    /**
     * @brief Sets fulltext filter.
     *
     * @param query
     */
    void setFilter(const QString &query);

    /**
     * @brief Returns fulltext filter string
     */
    QString filter() const;

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
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                             const QByteArray &rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CONTACTFETCHJOB_H
