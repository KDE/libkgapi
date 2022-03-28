/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PageDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    explicit PageDeleteJob(const PagePtr &page, const AccountPtr &account, QObject *parent = nullptr);
    explicit PageDeleteJob(const QString &blogId, const QString &pageId, const AccountPtr &account, QObject *parent = nullptr);
    ~PageDeleteJob() override;

protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};
}
}
