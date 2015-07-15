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

#ifndef KGAPI2_BLOGGER_POSTSEARCHJOB_H
#define KGAPI2_BLOGGER_POSTSEARCHJOB_H

#include "fetchjob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PostSearchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

    Q_PROPERTY(bool fetchBodies
               READ fetchBodies
               WRITE setFetchBodies)
  public:
    explicit PostSearchJob(const QString &blogId,
                           const QString &query,
                           const AccountPtr &account = AccountPtr(),
                           QObject *parent = 0);
    virtual ~PostSearchJob();

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

  protected:
    void start() Q_DECL_OVERRIDE;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) Q_DECL_OVERRIDE;

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_POSTSEARCHJOB_H
