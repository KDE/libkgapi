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

#ifndef KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H
#define KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H

#include "file.h"
#include "fileabstractdatajob.h"
#include "kgapidrive_export.h"

#include <QStringList>
#include <QMap>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileAbstractUploadJob : public KGAPI2::Drive::FileAbstractDataJob
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
                                   QObject *parent = nullptr);
    explicit FileAbstractUploadJob(const FilesList &metadata,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);
    explicit FileAbstractUploadJob(const QString &filePath,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);
    explicit FileAbstractUploadJob(const QString &filePath,
                                   const FilePtr &metaData,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);
    explicit FileAbstractUploadJob(const QStringList &filePaths,
                                   const AccountPtr &account,
                                   QObject *parent = nullptr);
    explicit FileAbstractUploadJob(const QMap < QString /* file path */,
                                   FilePtr /* metadata */ > &files,
                                   const AccountPtr &account, QObject *parent = nullptr);
    ~FileAbstractUploadJob() override;

    bool useContentAsIndexableText() const;
    void setUseContentAsIndexableText(bool useContentAsIndexableText);

    QMap < QString /* file path */, FilePtr /* metadata */ > files() const;

  protected:
    void start() override;
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;
    void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData) override;

    virtual QUrl createUrl(const QString &filePath, const FilePtr &metaData) = 0;
    virtual QNetworkReply *dispatch(QNetworkAccessManager *accessManager,
                                    const QNetworkRequest &request,
                                    const QByteArray &data) = 0;
    void setSerializationOptions(File::SerializationOptions options);
    File::SerializationOptions serializationOptions() const;

  private:
    class Private;
    Private *const d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_uploadProgress(qint64 uploadedBytes, qint64 totalBytes))

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEABSTRACTUPLOADJOB_H
