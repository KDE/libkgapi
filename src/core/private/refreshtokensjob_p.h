/*
 * This file is part of LibKGAPI
 *
 * SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "job.h"
#include "kgapicore_export.h"

namespace KGAPI2
{

class KGAPICORE_EXPORT RefreshTokensJob : public KGAPI2::Job
{
    Q_OBJECT
public:
    explicit RefreshTokensJob(const AccountPtr &account, const QString &apiKey, const QString &secretKey, QObject *parent = nullptr);

    ~RefreshTokensJob() override;

    AccountPtr account() const;

protected:
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request, const QByteArray &data, const QString &contentType) override;
    void start() override;

private:
    class Private;
    const QScopedPointer<Private> d;
    friend class Private;
};

} // namespace KGAPI2
