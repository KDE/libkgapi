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

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class FileSearchQuery;
class KGAPIDRIVE_EXPORT FileFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Whether to update the view date after successfully retrieving files.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateViewedDate READ updateViewedDate WRITE setUpdateViewedDate)

public:
    struct FieldShorthands {
        static const QStringList BasicFields;
        static const QStringList AccessFields;
        static const QStringList SharingFields;
    };

    explicit FileFetchJob(const QString &fileId, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const QStringList &filesIds, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const FileSearchQuery &query, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    ~FileFetchJob() override;

    void setFields(const QStringList &fields);
    QStringList fields() const;

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

    /**
     * @brief Whether both My Drive and shared drive items should be included in results.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards shared drive items will be included in the results.
     */
    KGAPIDRIVE_DEPRECATED bool includeItemsFromAllDrives() const;

    /**
     * @brief Sets whether both My Drive and shared drive items should be included in results.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards shared drive items will be included in the results.
     */
    KGAPIDRIVE_DEPRECATED void setIncludeItemsFromAllDrives(bool includeItemsFromAllDrives);

    /**
     * @brief Whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED bool supportsAllDrives() const;

    /**
     * @brief Sets whether the request supports both My Drives and shared drives.
     *
     * Set to true by default as LibKGAPI supports Team Drives.
     *
     * @deprecated This parameter will only be effective until June 1, 2020. Afterwards all applications
     * are assumed to support shared drives.
     */
    KGAPIDRIVE_DEPRECATED void setSupportsAllDrives(bool supportsAllDrives);

protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
