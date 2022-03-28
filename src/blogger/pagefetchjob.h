/*
 * <one line to give the library's name and an idea of what it does.>
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "fetchjob.h"
#include "kgapiblogger_export.h"
#include "page.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PageFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

public:
    enum StatusFilter {
        Draft = 1,
        Live = 2,
        Imported = 4,

        All = Draft | Live | Imported
    };
    Q_DECLARE_FLAGS(StatusFilters, StatusFilter)

    explicit PageFetchJob(const QString &blogId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    explicit PageFetchJob(const QString &blogId, const QString &pageId, const AccountPtr &account = AccountPtr(), QObject *parent = nullptr);
    ~PageFetchJob() override;

    bool fetchContent() const;
    void setFetchContent(bool fetchContent);

    StatusFilters statusFilter() const;
    void setStatusFilter(StatusFilters status);

protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

private:
    class Private;
    Private *const d;
    friend class Private;
};
}
}
