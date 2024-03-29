/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapiblogger_export.h"
#include "modifyjob.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PageModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

public:
    explicit PageModifyJob(const PagePtr &page, const AccountPtr &account, QObject *parent = nullptr);
    ~PageModifyJob() override;

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2
