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


#ifndef KGAPI2_DRIVEFILEMODIFYJOB_H
#define KGAPI2_DRIVEFILEMODIFYJOB_H

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

#endif // KGAPI2_DRIVEFILEMODIFYJOB_H
