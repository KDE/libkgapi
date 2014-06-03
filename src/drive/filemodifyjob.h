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


#ifndef KGAPI2_DRIVEFILEMODIFYJOB_H
#define KGAPI2_DRIVEFILEMODIFYJOB_H

#include <libkgapi2/drive/fileabstractuploadjob.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT FileModifyJob : public KGAPI2::Drive::FileAbstractUploadJob
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
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileModifyJob(const QString &filePath, const QString &fileId,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileModifyJob(const QString &filePath,
                           const FilePtr &metaData,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileModifyJob(const QMap < QString /* file path */, QString /* file ID */ > &files,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileModifyJob(const QMap < QString /* file path */, FilePtr /* metadata */ > &files,
                           const AccountPtr &account, QObject *parent = 0);
    virtual ~FileModifyJob();

    bool createNewRevision() const;
    void setCreateNewRevision(bool createNewRevision);

    bool updateModifiedDate() const;
    void setUpdateModifiedDate(bool updateModifiedDate);

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

  protected:
    virtual QNetworkReply *dispatch(QNetworkAccessManager *accessManager,
                                    const QNetworkRequest &request,
                                    const QByteArray &data);
    virtual QUrl createUrl(const QString &filePath,
                           const FilePtr &metaData);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEMODIFYJOB_H
