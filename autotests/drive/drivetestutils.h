/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "types.h"

KGAPI2::Drive::AboutPtr aboutFromFile(const QString &path);
KGAPI2::Drive::ChangePtr changeFromFile(const QString &path);
KGAPI2::Drive::FilePtr fileFromFile(const QString &path);
KGAPI2::Drive::DrivesPtr drivesFromFile(const QString &path);
KGAPI2::Drive::TeamdrivePtr teamdriveFromFile(const QString &path);
