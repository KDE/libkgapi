/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#pragma once

#include "fetchjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{
namespace Drive
{

class KGAPIDRIVE_EXPORT FileFetchContentJob : public KGAPI2::FetchJob
{
    Q_OBJECT

  public:
    explicit FileFetchContentJob(const FilePtr &file, const AccountPtr &account,
                                 QObject *parent = nullptr);
    explicit FileFetchContentJob(const QUrl &url, const AccountPtr &account,
                                 QObject *parent = nullptr);
    ~FileFetchContentJob() override;

    QByteArray data() const;

  protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;

    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

