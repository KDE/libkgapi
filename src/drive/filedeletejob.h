/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEFILEDELETEJOB_H
#define KGAPI2_DRIVEFILEDELETEJOB_H

#include "deletejob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit FileDeleteJob(const QString &fileId,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileDeleteJob(const QStringList &filesIds,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileDeleteJob(const FilePtr &file,
                           const AccountPtr &account, QObject *parent = nullptr);
    explicit FileDeleteJob(const FilesList &files,
                           const AccountPtr &account, QObject *parent = nullptr);
    ~FileDeleteJob() override;

  protected:
    void start() override;

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILEDELETEJOB_H
