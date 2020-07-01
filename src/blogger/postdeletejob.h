/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_BLOGGER_POSTDELETEJOB_H
#define KGAPI2_BLOGGER_POSTDELETEJOB_H

#include "deletejob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PostDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit PostDeleteJob(const QString &blogId,
                           const QString &postId,
                           const AccountPtr &account,
                           QObject *parent = nullptr);
    explicit PostDeleteJob(const PostPtr &post,
                           const AccountPtr &account,
                           QObject *parent = nullptr);
    ~PostDeleteJob() override;


  protected:
    void start() override;
    void handleReply(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;
    friend class Private;
};
}
}

#endif // KGAPI2_BLOGGER_POSTDELETEJOB_H
