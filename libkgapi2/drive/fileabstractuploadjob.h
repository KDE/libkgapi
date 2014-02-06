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

#ifndef KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H
#define KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H

#include <libkgapi2/drive/fileabstractdatajob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QStringList>
#include <QtCore/QMap>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT FileAbstractUploadJob : public KGAPI2::Drive::FileAbstractDataJob
{
    Q_OBJECT

    /**
     * Whether to use the content as indexable text.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool useContentAsIndexableText
               READ useContentAsIndexableText
               WRITE setUseContentAsIndexableText)

  public:
    explicit FileAbstractUploadJob(const FilePtr &metadata,
                                   const AccountPtr &account,
                                   QObject *parent = 0);
    explicit FileAbstractUploadJob(const FilesList &metadata,
                                   const AccountPtr &account,
                                   QObject *parent = 0);
    explicit FileAbstractUploadJob(const QString &filePath,
                                   const AccountPtr &account,
                                   QObject *parent = 0);
    explicit FileAbstractUploadJob(const QString &filePath,
                                   const FilePtr &metaData,
                                   const AccountPtr &account,
                                   QObject *parent = 0);
    explicit FileAbstractUploadJob(const QStringList &filePaths,
                                   const AccountPtr &account,
                                   QObject *parent = 0);
    explicit FileAbstractUploadJob(const QMap < QString /* file path */,
                                   FilePtr /* metadata */ > &files,
                                   const AccountPtr &account, QObject *parent = 0);
    virtual ~FileAbstractUploadJob();

    bool useContentAsIndexableText() const;
    void setUseContentAsIndexableText(bool useContentAsIndexableText);

    QMap < QString /* file path */, FilePtr /* metadata */ > files() const;

  protected:
    virtual void start();
    virtual void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType);
    virtual void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData);

    virtual QUrl createUrl(const QString &filePath, const FilePtr &metaData) = 0;
    virtual QNetworkReply *dispatch(QNetworkAccessManager *accessManager,
                                    const QNetworkRequest &request,
                                    const QByteArray &data) = 0;

  private:
    class Private;
    Private *const d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_uploadProgress(qint64 uploadedBytes, qint64 totalBytes))

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H
