/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@kde.org>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 *
 */

#pragma once

#include "fetchjob.h"
#include "kgapipeople_export.h"

namespace KGAPI2::People
{

class KGAPIPEOPLE_EXPORT ContactGroupFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

public:
    explicit ContactGroupFetchJob(const AccountPtr &account, QObject *parent = nullptr);
    ContactGroupFetchJob(const QString &resourceName,
                         const AccountPtr &account,
                         QObject* parent = nullptr); // Use the resourceName as an id for the contact group
    ~ContactGroupFetchJob() override;

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                     const QByteArray &rawData) override;

private:
    class Private;
    Private * const d;
    friend class Private;
};

}
