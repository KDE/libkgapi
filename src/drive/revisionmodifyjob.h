/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "modifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT RevisionModifyJob : public KGAPI2::ModifyJob
{
    Q_OBJECT

  public:
    explicit RevisionModifyJob(const QString &fileId,
                               const RevisionPtr &revision,
                               const AccountPtr &account, QObject *parent = nullptr);
    explicit RevisionModifyJob(const QString &fileId,
                               const RevisionsList &revisions,
                               const AccountPtr &account, QObject *parent = nullptr);
    ~RevisionModifyJob() override;

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

