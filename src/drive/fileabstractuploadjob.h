/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
