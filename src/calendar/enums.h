/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2018 Daniel Vrátil <dvratil@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapicalendar_export.h"


namespace KGAPI2 {

Q_NAMESPACE_EXPORT(KGAPICALENDAR_EXPORT)

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

