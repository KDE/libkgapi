/*
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
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLOGFETCHJOB_H
#define BLOGFETCHJOB_H

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
                          QObject *parent = 0);
    virtual ~BlogFetchJob();

  protected:
    void start() Q_DECL_OVERRIDE;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;
};

} // namespace Blogger
} // namespace KGAPI2

#endif // BLOGFETCHJOB_H
