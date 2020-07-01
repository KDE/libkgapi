/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KGAPI2_DRIVECHILDREFERENCEDELETEJOB_H
#define KGAPI2_DRIVECHILDREFERENCEDELETEJOB_H

#include "deletejob.h"
#include "kgapidrive_export.h"

#include <QStringList>

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT ChildReferenceDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    explicit ChildReferenceDeleteJob(const QString &folderId,
                                     const QString &childId,
                                     const AccountPtr &account,
                                     QObject *parent = nullptr);
    explicit ChildReferenceDeleteJob(const QString &folderId,
                                     const QStringList &childrenIds,
                                     const AccountPtr &account,
                                     QObject *parent = nullptr);
    explicit ChildReferenceDeleteJob(const QString &folderId,
                                     const ChildReferencePtr &reference,
                                     const AccountPtr &account,
                                     QObject *parent = nullptr);
    explicit ChildReferenceDeleteJob(const QString &folderId,
                                     const ChildReferencesList &references,
                                     const AccountPtr &account,
                                     QObject *parent = nullptr);
    ~ChildReferenceDeleteJob() override;

  protected:
    void start() override;

  private:
    class Private;
    Private *const d;
    friend class Private;

};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVECHILDREFERENCEDELETEJOB_H
