/*
 * Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "driveservice.h"
#include "about.h"
#include "change.h"
#include "file.h"
#include "teamdrive.h"
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

KGAPI2::Drive::TeamdrivePtr teamdriveFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto teamdrive = KGAPI2::Drive::Teamdrive::fromJSON(f.readAll());
    VERIFY_RET(teamdrive, {});
    return teamdrive;
}
