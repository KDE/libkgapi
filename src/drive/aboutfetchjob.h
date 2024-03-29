/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

/**
 * @brief A fetch job that retrieves DriveAbout data.
 */
class KGAPIDRIVE_EXPORT AboutFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * @brief Whether to include shared and public files to change IDs count
     *
     * When calculating the number of remaining change IDs, whether to include
     * shared files and public files the user has opened. When set to false, this
     * counts only change IDs for owned files and any shared or public files that
     * the user has explicitly added to a folder in Drive.
     *
     * Default value is true.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool includeSubscribed READ includeSubscribed WRITE setIncludeSubscribed)

    /**
     * @brief Maximum number of remaining change IDs to count.
     *
     * Default value is 0, i.e. no limit
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(qlonglong maxChangeIdCount READ maxChangeIdCount WRITE setMaxChangeIdCount)

    /**
     * @brief Change ID to start counting from when calculating number of
     *        remaining change IDs.
     *
     * Default value is 0, i.e. first ID
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(qlonglong startChangeId READ startChangeId WRITE setStartChangeId)

public:
    /**
     * @brief Constructs a new fetch job.
     *
     * The @p account must be authenticated with one of the following scopes:
     * <ul>
     * <li>https://www.googleapis.com/auth/drive.readonly.metadata</li>
     * <li>https://www.googleapis.com/auth/drive.readonly</li>
     * <li>https://www.googleapis.com/auth/drive</li>
     * <li>https://www.googleapis.com/auth/drive.file</li>
     * <li>https://www.googleapis.com/auth/drive.metadata.readonly</li>
     * </ul>
     *
     * @param account Account to authenticate the request with
     * @param parent
     */
    explicit AboutFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    ~AboutFetchJob() override;

    /**
     * @brief Returns whether to include shared and public files to change IDs count.
     */
    [[nodiscard]] bool includeSubscribed() const;

    /**
     * @brief Sets whether to include shared and public files to change IDs count.
     *
     * @param includeSubscribed
     */
    void setIncludeSubscribed(bool includeSubscribed);

    /**
     * @brief Returns maximum number of remaining change IDs to count.
     */
    [[nodiscard]] qlonglong maxChangeIdCount() const;

    /**
     * @brief Sets maximum number of remaining change IDs to count.
     *
     * @param maxChangeIdCount
     */
    void setMaxChangeIdCount(qlonglong maxChangeIdCount);

    /**
     * @brief Returns change ID to start counting from when calculating number of
     *        remaining change IDs.
     */
    [[nodiscard]] qlonglong startChangeId() const;

    /**
     * @brief Sets change ID to start counting from when calculating number of
     *        remaining change IDs.
     *
     * @param startChangeId
     */
    void setStartChangeId(qlonglong startChangeId);

    /**
     * @brief Returns the retrieved DriveAbout object
     *
     * This method can only be called after the job has emitted finished()
     */
    [[nodiscard]] AboutPtr aboutData() const;

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
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
