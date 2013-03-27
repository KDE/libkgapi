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

#ifndef KGAPI2_DRIVEFILECOPYJOB_H
#define KGAPI2_DRIVEFILECOPYJOB_H

#include <libkgapi2/drive/fileabstractdatajob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QMap>

namespace KGAPI2
{

class LIBKGAPI2_EXPORT DriveFileCopyJob : public KGAPI2::DriveFileAbstractDataJob
{
    Q_OBJECT

  public:
    explicit DriveFileCopyJob(const QString &sourceFileId,
                              const DriveFilePtr &destinationFile,
                              const AccountPtr &account, QObject *parent = 0);
    explicit DriveFileCopyJob(const DriveFilePtr &sourceFile,
                              const DriveFilePtr &destinationFile,
                              const AccountPtr &account, QObject *parent = 0);
    explicit DriveFileCopyJob(const QMap< QString /* source file id */,
                                          DriveFilePtr /* destination file */ > &files,
                              const AccountPtr &account, QObject *parent = 0);
    explicit DriveFileCopyJob(const QMap< DriveFilePtr /* source file */,
                                          DriveFilePtr /* destination file */ > &files,
                              const AccountPtr &account, QObject *parent = 0);
    virtual ~DriveFileCopyJob();

    DriveFilesList files() const;

  protected:
    virtual void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData);

    virtual void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType);
    virtual void start();

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILECOPYJOB_H
