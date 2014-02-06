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

#ifndef LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H
#define LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H

#include <libkgapi2/job.h>
#include <libkgapi2/libkgapi2_export.h>

namespace KGAPI2 {

/**
 * @internal
 *
 * Exported, but not publicly avilable!
 */
class LIBKGAPI2_EXPORT NewTokensFetchJob : public KGAPI2::Job
{
    Q_OBJECT

  public:
    explicit NewTokensFetchJob(const QString &tmpToken, const QString &apiKey, const QString &secretKey, QObject* parent = 0);
    virtual ~NewTokensFetchJob();

    QString accessToken() const;
    QString refreshToken() const;
    qulonglong expiresIn() const;

  protected:
    virtual void start();
    virtual void handleReply(const QNetworkReply *reply, const QByteArray& rawData);
    virtual void dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType);

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H
