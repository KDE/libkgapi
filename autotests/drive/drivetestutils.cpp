/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "driveservice.h"
#include "about.h"
#include "change.h"
#include "file.h"
#include "teamdrive.h"
#include "drives.h"
#include "testutils.h"

#include <QFile>

KGAPI2::Drive::AboutPtr aboutFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto about = KGAPI2::Drive::About::fromJSON(f.readAll());
    VERIFY_RET(about, {});
    return about;
}

KGAPI2::Drive::ChangePtr changeFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto change = KGAPI2::Drive::Change::fromJSON(f.readAll());
    VERIFY_RET(change, {});
    return change;
}

KGAPI2::Drive::FilePtr fileFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto file = KGAPI2::Drive::File::fromJSON(f.readAll());
    VERIFY_RET(file, {});
    return file;
}

KGAPI2::Drive::DrivesPtr drivesFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto drives = KGAPI2::Drive::Drives::fromJSON(f.readAll());
    VERIFY_RET(drives, {});
    return drives;
}

KGAPI2::Drive::TeamdrivePtr teamdriveFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto teamdrive = KGAPI2::Drive::Teamdrive::fromJSON(f.readAll());
    VERIFY_RET(teamdrive, {});
    return teamdrive;
}
