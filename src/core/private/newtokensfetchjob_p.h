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

#ifndef LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H
#define LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H

#include "job.h"
#include "kgapicore_export.h"

namespace KGAPI2 {

/**
 * @internal
 *
 * Exported, but not publicly avilable!
 */
class KGAPICORE_EXPORT NewTokensFetchJob : public KGAPI2::Job
{
    Q_OBJECT

  public:
    explicit NewTokensFetchJob(const QString &tmpToken, const QString &apiKey, const QString &secretKey, QObject* parent = nullptr);
    virtual ~NewTokensFetchJob();

    QString accessToken() const;
    QString refreshToken() const;
    qulonglong expiresIn() const;

  protected:
    void start() Q_DECL_OVERRIDE;
    void handleReply(const QNetworkReply *reply, const QByteArray& rawData) Q_DECL_OVERRIDE;
    void dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_PRIVATE_NEWTOKENSFETCHJOB_P_H
