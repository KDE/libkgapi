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

#ifndef KGAPI2_DRIVEFILEFETCHJOB_H
#define KGAPI2_DRIVEFILEFETCHJOB_H

#include <libkgapi2/fetchjob.h>
#include <libkgapi2/libkgapi2_export.h>

#include <QtCore/QStringList>

namespace KGAPI2
{

namespace Drive
{

class LIBKGAPI2_EXPORT FileFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    /**
     * Whether to update the view date after successfully retrieving files.
     *
     * Default value is false.
     *
     * This property can be modified only when the job is not running.
     */
    Q_PROPERTY(bool updateViewedDate
               READ updateViewedDate
               WRITE setUpdateViewedDate)

  public:
    explicit FileFetchJob(const QString &fileId,
                          const AccountPtr &account, QObject *parent = 0);
    explicit FileFetchJob(const QStringList &filesIds,
                          const AccountPtr &account, QObject *parent = 0);
    explicit FileFetchJob(const AccountPtr &account, QObject *parent = 0);
    virtual ~FileFetchJob();

    bool updateViewedDate() const;
    void setUpdateViewedDate(bool updateViewedDate);

  protected:
    virtual void start();
    virtual KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData);

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEFETCHJOB_H
