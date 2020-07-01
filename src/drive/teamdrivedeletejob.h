/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#ifndef KGAPI2_DRIVETEAMDRIVEDELETEJOB_H
#define KGAPI2_DRIVETEAMDRIVEDELETEJOB_H

#include "deletejob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_DEPRECATED_EXPORT TeamdriveDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

  public:
    TeamdriveDeleteJob(const QString &teamdriveId,
                        const AccountPtr &account, QObject *parent = nullptr);
    TeamdriveDeleteJob(const QStringList &teamdrivesIds,
                        const AccountPtr &account, QObject *parent = nullptr);
    TeamdriveDeleteJob(const TeamdrivePtr &teamdrive,
                        const AccountPtr &account, QObject *parent = nullptr);
    TeamdriveDeleteJob(const TeamdrivesList &teamdrives,
                        const AccountPtr &account, QObject *parent = nullptr);
    ~TeamdriveDeleteJob() override;

  protected:
    void start() override;

  private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2

#endif // KGAPI2_DRIVETEAMDRIVEDELETEJOB_H
