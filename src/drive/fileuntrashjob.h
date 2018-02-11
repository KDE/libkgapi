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

#ifndef KGAPI2_DRIVEFILEUNTRASHJOB_H
#define KGAPI2_DRIVEFILEUNTRASHJOB_H

#include "fileabstractmodifyjob.h"
#include "kgapidrive_export.h"

class QNetworkAccessManager;
class QNetworkRequest;

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileUntrashJob : public KGAPI2::Drive::FileAbstractModifyJob
{
    Q_OBJECT

  public:
    explicit FileUntrashJob(const QString &fileId,
                            const AccountPtr &account, QObject *parent = nullptr);
    explicit FileUntrashJob(const QStringList &filesIds,
                            const AccountPtr &account, QObject *parent = nullptr);
    explicit FileUntrashJob(const FilePtr &file,
                            const AccountPtr &account, QObject *parent = nullptr);
    explicit FileUntrashJob(const FilesList &files,
                            const AccountPtr &account, QObject *parent = nullptr);
    ~FileUntrashJob() override;

  protected:
    QUrl url(const QString &fileId) override;
    void dispatchRequest(QNetworkAccessManager *accessManager,
                                 const QNetworkRequest &request,
                                 const QByteArray &data,
                                 const QString &contentType) override;

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEUNTRASHJOB_H
