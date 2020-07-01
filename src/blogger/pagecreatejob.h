/*
 * SPDX-FileCopyrightText: 2014 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_BLOGGER_PAGECREATEJOB_H
#define KGAPI2_BLOGGER_PAGECREATEJOB_H

#include "createjob.h"
#include "kgapiblogger_export.h"

namespace KGAPI2
{
namespace Blogger
{

class KGAPIBLOGGER_EXPORT PageCreateJob : public KGAPI2::CreateJob
{
    Q_OBJECT

  public:
    explicit PageCreateJob(const PagePtr &page,
                           const AccountPtr &account,
                           QObject *parent = nullptr);
    ~PageCreateJob() override;

  protected:
    void start() override;
    ObjectsList handleReplyWithItems(const QNetworkReply *reply, const QByteArray &rawData) override;

  private:
    class Private;
    Private * const d;

};
}
}

#endif // KGAPI2_BLOGGER_PAGECREATEJOB_H
