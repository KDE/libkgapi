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


#ifndef KGAPI2_DRIVE_FILEFETCHCONTENTJOB_H
#define KGAPI2_DRIVE_FILEFETCHCONTENTJOB_H

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
                                 QObject *parent = 0);
    explicit FileFetchContentJob(const QUrl &url, const AccountPtr &account,
                                 QObject *parent = 0);
    virtual ~FileFetchContentJob();

    QByteArray data() const;

  protected:
    virtual void start();
    virtual void handleReply(const QNetworkReply *reply, const QByteArray &rawData);
    virtual void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType);

    virtual KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                                     const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;

    Q_PRIVATE_SLOT(d, void _k_downloadProgress(qint64 downloaded, qint64 total))
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVE_FILEFETCHCONTENTJOB_H
