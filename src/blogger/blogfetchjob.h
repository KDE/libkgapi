/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{

namespace Blogger
{

class KGAPIBLOGGER_EXPORT BlogFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

  public:
    enum FetchBy {
        FetchByBlogId,
        FetchByBlogUrl,
        FetchByUserId
    };
    explicit BlogFetchJob(const QString &id,
                          FetchBy fetchBy,
                          const AccountPtr &account = AccountPtr(),
                          QObject *parent = nullptr);
    ~BlogFetchJob() override;

  protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

