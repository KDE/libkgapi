/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef CALENDARTESTUTILS_H_
#define CALENDARTESTUTILS_H_

#include "types.h"
#include "calendar.h"
#include "event.h"

KGAPI2::CalendarPtr calendarFromFile(const QString &path);
KGAPI2::EventPtr eventFromFile(const QString &path);

#endif

