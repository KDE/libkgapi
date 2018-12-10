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
