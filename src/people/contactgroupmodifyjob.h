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

class KGAPIPEOPLE_EXPORT ContactGroupModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

public:
    explicit ContactGroupModifyJob(const ContactGroupPtr &contactGroup, const AccountPtr &account, QObject* parent = nullptr);
    explicit ContactGroupModifyJob(const ContactGroupList &contactGroups, const AccountPtr &account, QObject* parent = nullptr);
    ~ContactGroupModifyJob();

protected:
    void start() override;

    ObjectsList handleReplyWithItems(const QNetworkReply *reply,
                                     const QByteArray &rawData) override;

private:
    class Private;
    std::unique_ptr<Private> d;
    friend class Private;
};

}
