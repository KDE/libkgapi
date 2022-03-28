/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fileabstractmodifyjob.h"
#include "kgapidrive_export.h"

class QNetworkAccessManager;
class QNetworkRequest;

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileTrashJob : public KGAPI2::Drive::FileAbstractModifyJob
{
    Q_OBJECT

public:
    explicit FileTrashJob(const QString &fileId, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileTrashJob(const QStringList &filesIds, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileTrashJob(const FilePtr &file, const AccountPtr &account, QObject *parent = nullptr);
    explicit FileTrashJob(const FilesList &files, const AccountPtr &account, QObject *parent = nullptr);
    ~FileTrashJob() override;

protected:
    QUrl url(const QString &fileId) override;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
