/*
 * This file is part of LibKGAPI
 *
 * SPDX-FileCopyrightText: 2020 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LIBKGAPI2_FULLAUTHENTICATIONJOB_P_H_
#define LIBKGAPI2_FULLAUTHENTICATIONJOB_P_H_

#include "job.h"
#include "kgapicore_export.h"

namespace KGAPI2
{

class KGAPICORE_EXPORT FullAuthenticationJob : public KGAPI2::Job
{
    Q_OBJECT
public:
    explicit FullAuthenticationJob(const AccountPtr &account, const QString &apiKey, const QString &secretKey, QObject *parent = nullptr);

    ~FullAuthenticationJob() override;

    AccountPtr account() const;

    /** For testing purposes only */
    void setServerPort(uint16_t port);

    void setUsername(const QString &username);

protected:
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;
    void dispatchRequest(QNetworkAccessManager *accessManager, const QNetworkRequest &request,
                         const QByteArray &data, const QString &contentType) override;
    void start() override;

private:
    class Private;
    const QScopedPointer<Private> d;
    friend class Private;
};


} // namespace KGAPI2

#endif


