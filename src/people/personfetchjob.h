/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapipeople_export.h"

namespace KGAPI2::People
{

class KGAPIPEOPLE_EXPORT PersonFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT
    Q_PROPERTY(QString syncToken READ syncToken WRITE setSyncToken NOTIFY syncTokenChanged)
    Q_PROPERTY(QString receivedSyncToken READ receivedSyncToken NOTIFY receivedSyncTokenChanged)

public:
    explicit PersonFetchJob(const AccountPtr &account, QObject* parent = nullptr);
    explicit PersonFetchJob(const QString &resourceName, const AccountPtr &account, QObject* parent = nullptr);
    ~PersonFetchJob();

    [[nodiscard]] QString syncToken() const;
    [[nodiscard]] QString receivedSyncToken() const;

public Q_SLOTS:
    void setSyncToken(const QString &syncToken);

Q_SIGNALS:
    void syncTokenChanged();
    void receivedSyncTokenChanged();

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                     const QByteArray &rawData) override;
    bool handleError(int statusCode, const QByteArray &rawData) override;

private:
    class Private;
    std::unique_ptr<Private> d;
    friend class Private;
};

}
