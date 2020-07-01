/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_BLOGGER_POSTMODIFYJOB_H
#define KGAPI2_BLOGGER_POSTMODIFYJOB_H

#include "modifyjob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PostModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    explicit PostModifyJob(const PostPtr &post,
                           const AccountPtr &account,
                           QObject *parent = nullptr);
    ~PostModifyJob() override;

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

#endif // KGAPI2_BLOGGER_POSTMODIFYJOB_H
