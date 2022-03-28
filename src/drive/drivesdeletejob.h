/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapidrive_export.h"

namespace KGAPI2
{

namespace Drive
{

class KGAPIDRIVE_EXPORT DrivesDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    DrivesDeleteJob(const QString &drivesId, const AccountPtr &account, QObject *parent = nullptr);
    DrivesDeleteJob(const QStringList &drivesIds, const AccountPtr &account, QObject *parent = nullptr);
    DrivesDeleteJob(const DrivesPtr &drives, const AccountPtr &account, QObject *parent = nullptr);
    DrivesDeleteJob(const DrivesList &drives, const AccountPtr &account, QObject *parent = nullptr);
    ~DrivesDeleteJob() override;

protected:
    void start() override;

private:
    class Private;
    QScopedPointer<Private> d;
    friend class Private;
};

} // namespace Drive

} // namespace KGAPI2
