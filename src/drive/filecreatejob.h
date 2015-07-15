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

#ifndef KGAPI2_DRIVEFILECREATEJOB_H
#define KGAPI2_DRIVEFILECREATEJOB_H

#include "fileabstractuploadjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileCreateJob : public KGAPI2::Drive::FileAbstractUploadJob
{
    Q_OBJECT

  public:
    explicit FileCreateJob(const FilePtr &metadata, const AccountPtr &account,
                           QObject *parent = 0);
    explicit FileCreateJob(const FilesList &metadata, const AccountPtr &account,
                           QObject *parent = 0);
    explicit FileCreateJob(const QString &filePath,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileCreateJob(const QString &filePath,
                           const FilePtr &metaData,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileCreateJob(const QStringList &filePaths,
                           const AccountPtr &account, QObject *parent = 0);
    explicit FileCreateJob(const QMap < QString /* filepath */, FilePtr /* metadata */ > &files,
                           const AccountPtr &account, QObject *parent = 0);
    virtual ~FileCreateJob();

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

#endif // KGAPI2_DRIVEFILECREATEJOB_H
