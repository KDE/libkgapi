/*
 * This file is part of LibKGAPI library
 *
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

#ifndef LIBKGAPI2_ENUMS_H_
#define LIBKGAPI2_ENUMS_H_

#include "kgapicalendar_export.h"

#include <QObject>

namespace KGAPI2 {

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
Q_NAMESPACE_EXPORT(KGAPICALENDAR_EXPORT)
#else
KGAPICALENDAR_EXPORT Q_NAMESPACE
#endif

/**
 * Determines whether Google Calendar should send updates to participants
 */
enum class SendUpdatesPolicy
{
    All,    /// Send updates to all attendees
    ExternalOnly, /// Only send updates to non-Google Calendar participants
    None, /// Don't send any notifications
};

Q_ENUM_NS(KGAPI2::SendUpdatesPolicy)

} // namespace KGAPI

#endif // LIBKGAPI2_ENUMS_H_
