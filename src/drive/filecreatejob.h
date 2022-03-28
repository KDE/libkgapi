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

class KGAPIDRIVE_EXPORT FileCreateJob : public KGAPI2::Drive::FileAbstractUploadJob
{
    Q_OBJECT

public:
    explicit FileCreateJob(const FilePtr &metadata, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCreateJob(const FilesList &metadata, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCreateJob(const QString &filePath, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCreateJob(const QString &filePath, const FilePtr &metaData, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCreateJob(const QStringList &filePaths, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileCreateJob(const QMap<QString /* filepath */, FilePtr /* metadata */> &files, const AccountPtr &account, QObject *parent = nullptr);
    ~FileCreateJob() override;

protected:
    QNetworkReply *dispatch(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data) override;

    QUrl createUrl(const QString &filePath, const FilePtr &metaData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
