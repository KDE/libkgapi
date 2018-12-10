/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef KGAPI2_BLOGGER_PAGEFETCHJOB_H
#define KGAPI2_BLOGGER_PAGEFETCHJOB_H

#include "fetchjob.h"
#include "page.h"
#include "kgapiblogger_export.h"

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

    explicit PageFetchJob(const QString &blogId,
                          const AccountPtr &account = AccountPtr(),
                          QObject *parent = nullptr);
    explicit PageFetchJob(const QString &blogId,
                          const QString &pageId,
                          const AccountPtr &account = AccountPtr(),
                          QObject *parent = nullptr);
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
    Private * const d;
    friend class Private;

};
}
}

#endif // KGAPI2_BLOGGER_PAGEFETCHJOB_H
