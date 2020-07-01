/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEFILECOPYJOB_H
#define KGAPI2_DRIVEFILECOPYJOB_H

#include "fileabstractdatajob.h"
#include "kgapidrive_export.h"

#include <QMap>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileCopyJob : public KGAPI2::Drive::FileAbstractDataJob
{
    Q_OBJECT

  public:
    explicit FileCopyJob(const QString &sourceFileId,
                         const FilePtr &destinationFile,
                         const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCopyJob(const FilePtr &sourceFile,
                         const FilePtr &destinationFile,
                         const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCopyJob(const QMap < QString /* source file id */,
                         FilePtr /* destination file */ > &files,
                         const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCopyJob(const QMap < FilePtr /* source file */,
                         FilePtr /* destination file */ > &files,
                         const AccountPtr &account, QObject *parent = nullptr);
    ~FileCopyJob() override;

    FilesList files() const;

  protected:
    void handleReply(const QNetworkReply *reply,
                             const QByteArray &rawData) override;

    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;
    void start() override;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILECOPYJOB_H
