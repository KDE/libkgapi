/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#pragma once

#include "fileabstractuploadjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileModifyJob : public KGAPI2::Drive::FileAbstractUploadJob
{
    Q_OBJECT

    /**
     * Whether a blob upload should create a new revision. If set to false,
     * the blob data in the current head revision is replaced. If true, a new
     * blob is created as head revision, and previous revisions are preserved
     * (causing increased use of the user's data storage quota).
     *
     * Default value is true.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool createNewRevision
               READ createNewRevision
               WRITE setCreateNewRevision)

    /**
     * Whether to set the modified date with the supplied modified date.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateModifiedDate
               READ updateModifiedDate
               WRITE setUpdateModifiedDate)

    /**
     * Whether to update the view date after successfully updating the file.
     *
     * Default value is true.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateViewedDate
               READ updateViewedDate
               WRITE setUpdateViewedDate)

  public:
    explicit FileModifyJob(const FilePtr &metadata,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileModifyJob(const QString &filePath, const QString &fileId,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileModifyJob(const QString &filePath,
                           const FilePtr &metaData,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileModifyJob(const QMap < QString /* file path */, QString /* file ID */ > &files,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileModifyJob(const QMap < QString /* file path */, FilePtr /* metadata */ > &files,
                           const AccountPtr &account, QObject *parent = nullptr);
    ~FileModifyJob() override;

    bool createNewRevision() const;
    void setCreateNewRevision(bool createNewRevision);

    bool updateModifiedDate() const;
    void setUpdateModifiedDate(bool updateModifiedDate);

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

  protected:
    QNetworkReply *dispatch(QNetworkAccessManager *accessManager,
                                    const QNetworkRequest &request,
                                    const QByteArray &data) override;
    QUrl createUrl(const QString &filePath,
                           const FilePtr &metaData) override;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

