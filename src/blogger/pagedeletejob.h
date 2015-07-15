/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KGAPI2_BLOGGER_PAGEDELETEJOB_H
#define KGAPI2_BLOGGER_PAGEDELETEJOB_H

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
    explicit PageDeleteJob(const PagePtr &page,
                           const AccountPtr &account,
                           QObject *parent = 0);
    explicit PageDeleteJob(const QString &blogId,
                           const QString &pageId,
                           const AccountPtr &account,
                           QObject *parent = 0 );
    virtual ~PageDeleteJob();

  protected:
    void start();
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData);

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_PAGEDELETEJOB_H
