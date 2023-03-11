/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "job.h"
#include "kgapicore_export.h"

namespace KGAPI2
{

/**
 * @internal
 *
 * Exported, but not publicly available!
 */
class KGAPICORE_EXPORT NewTokensFetchJob : public KGAPI2::Job
{
    Q_OBJECT

public:
    explicit NewTokensFetchJob(const QString &tmpToken, const QString &apiKey, const QString &secretKey, int localPort, QObject *parent = nullptr);
    ~NewTokensFetchJob() override;

    Q_REQUIRED_RESULT QString accessToken() const;
    Q_REQUIRED_RESULT QString refreshToken() const;
    Q_REQUIRED_RESULT qulonglong expiresIn() const;

protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace KGAPI2
