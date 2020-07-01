/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVEFILETOUCHJOB_H
#define KGAPI2_DRIVEFILETOUCHJOB_H

#include "fileabstractmodifyjob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT FileTouchJob : public KGAPI2::Drive::FileAbstractModifyJob
{
    Q_OBJECT

  public:
    explicit FileTouchJob(const QString &fileId, const AccountPtr &account,
                          QObject *parent = nullptr);
    explicit FileTouchJob(const QStringList &filesIds,
                          const AccountPtr &account, QObject *parent = nullptr);
    explicit FileTouchJob(const FilePtr &file,
                          const AccountPtr &account, QObject *parent = nullptr);
    explicit FileTouchJob(const FilesList &files,
                          const AccountPtr &account, QObject *parent = nullptr);
    ~FileTouchJob() override;

  protected:
    QUrl url(const QString &fileId) override;

  private:
    class Private;
    Private *const d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVEFILETOUCHJOB_H
