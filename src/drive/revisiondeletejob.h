/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT RevisionDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit RevisionDeleteJob(const QString &fileId, const QString &revisionId,
                               const AccountPtr &account, QObject *parent = nullptr);
    explicit RevisionDeleteJob(const QString &fileId, const QStringList &revisionsIds,
                               const AccountPtr &account, QObject *parent = nullptr);
    explicit RevisionDeleteJob(const QString &fileId, const RevisionPtr &revision,
                               const AccountPtr &account, QObject *parent = nullptr);
    explicit RevisionDeleteJob(const QString &fileId, const RevisionsList &revisions,
                               const AccountPtr &account, QObject *parent = nullptr);
    ~RevisionDeleteJob() override;

  protected:
    void start() override;

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

