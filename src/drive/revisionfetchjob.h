/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEREVISIONFETCHJOB_H
#define KGAPI2_DRIVEREVISIONFETCHJOB_H

#include "fetchjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT RevisionFetchJob : public KGAPI2::FetchJob
{
    Q_OBJECT

  public:
    explicit RevisionFetchJob(const QString &fileId, const AccountPtr &account,
                              QObject *parent = nullptr);
    explicit RevisionFetchJob(const QString &fileId, const QString &revisionId,
                              const AccountPtr &account, QObject *parent = nullptr);
    ~RevisionFetchJob() override;

  protected:
    void start() override;
    KGAPI2::ObjectsList handleReplyWithItems(const QNetworkReply *reply,
            const QByteArray &rawData) override;

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEREVISIONFETCHJOB_H
