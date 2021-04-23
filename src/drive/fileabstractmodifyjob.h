/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "modifyjob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileAbstractModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    explicit FileAbstractModifyJob(const QString &fileId,
                                   const AccountPtr &account, QObject *parent = nullptr);
    explicit FileAbstractModifyJob(const QStringList &filesIds,
                                   const AccountPtr &account, QObject *parent = nullptr);
    explicit FileAbstractModifyJob(const FilePtr &file,
                                   const AccountPtr &account, QObject *parent = nullptr);
    explicit FileAbstractModifyJob(const FilesList &files,
                                   const AccountPtr &account, QObject *parent = nullptr);
    ~FileAbstractModifyJob() override;

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
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData) override;

    virtual QUrl url(const QString &fileId) = 0;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

