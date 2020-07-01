/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
                           QObject *parent = nullptr);
    ~PostSearchJob() override;

    bool fetchBodies() const;
    void setFetchBodies(bool fetchBodies);

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

#endif // KGAPI2_BLOGGER_POSTSEARCHJOB_H
