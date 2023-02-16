/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "modifyjob.h"
#include "kgapipeople_export.h"

namespace KGAPI2::People
{

class KGAPIPEOPLE_EXPORT PersonModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

public:
    explicit PersonModifyJob(const PersonPtr &person, const AccountPtr &account, QObject* parent = nullptr);
    explicit PersonModifyJob(const PersonList &people, const AccountPtr &account, QObject* parent = nullptr);
    ~PersonModifyJob();

protected:
    void start() override;

    void dispatchRequest(QNetworkAccessManager *accessManager,
                         const QNetworkRequest &request,
                         const QByteArray &data,
                         const QString &contentType) override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                     const QByteArray &rawData) override;

private:
    class Private;
    std::unique_ptr<Private> d;
    friend class Private;
};

}
