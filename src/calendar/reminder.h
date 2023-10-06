/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2013 Daniel Vrátil <dvratil@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "kgapicalendar_export.h"
#include "object.h"
#include "types.h"

#include <KCalendarCore/Alarm>
#include <KCalendarCore/Incidence>

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief Represents a default calendar reminder.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.4
 */
class KGAPICALENDAR_EXPORT Reminder
{
public:
    /**
     * @brief Constructor
     */
    explicit Reminder();

    /**
     * @brief Constructor
     *
     * @param type Type of the reminder (email, notification, etc.)
     * @param startOffset How long before the event should the reminder be shown
     */
    explicit Reminder(KCalendarCore::Alarm::Type type, const KCalendarCore::Duration &startOffset = KCalendarCore::Duration(0));

    /**
     * @brief Copy constructor
     */
    Reminder(const Reminder &other);

    /**
     * @brief Destructor
     */
    virtual ~Reminder();

    bool operator==(const Reminder &other) const;

    /**
     * @brief Returns type of the reminder
     */
    [[nodiscard]] KCalendarCore::Alarm::Type type() const;

    /**
     * @brief Sets type of the reminder
     *
     * @param type
     */
    void setType(KCalendarCore::Alarm::Type type);

    /**
     * @brief Returns how long before the event should reminder be shown
     */
    [[nodiscard]] KCalendarCore::Duration startOffset() const;

    /**
     * @brief Sets how long before the event should reminder be shown
     */
    void setStartOffset(const KCalendarCore::Duration &startOffset);

    /**
     * @brief Converts the reminder to a KCalendarCore::Alarm
     *
     * @param incidence An incidence on which the reminder should be applied
     * @return Returns a new KCalendarCore::Alarm
     */
    [[nodiscard]] KCalendarCore::Alarm *toAlarm(KCalendarCore::Incidence *incidence) const;

private:
    class Private;
    QScopedPointer<Private> const d;
};

} // namespace KGAPI2
