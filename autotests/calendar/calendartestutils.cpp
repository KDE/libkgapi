/*
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "calendartestutils.h"
#include "calendarservice.h"
#include "testutils.h"

#include <QFile>

KGAPI2::CalendarPtr calendarFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto calendar = KGAPI2::CalendarService::JSONToCalendar(f.readAll());
    VERIFY_RET(calendar, {});
    return calendar;
}

KGAPI2::EventPtr eventFromFile(const QString &path)
{
    QFile f(path);
    VERIFY_RET(f.open(QIODevice::ReadOnly), {});

    auto event = KGAPI2::CalendarService::JSONToEvent(f.readAll());
    VERIFY_RET(event, {});
    return event;
}
