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

#include <KCalendarCore/Event>

#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief Represents a single event from Google Calendar
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KGAPICALENDAR_EXPORT Event : public KGAPI2::Object, public KCalendarCore::Event
{
public:
    /**
     * @brief Constructor
     */
    Event();

    /**
     * @brief Copy constructor
     */
    Event(const Event &other);

    /**
     * @brief Copy constructor
     */
    explicit Event(const KCalendarCore::Event &event);

    /**
     * @brief Destructor
     */
    ~Event() override;

    bool operator==(const Event &other) const;

    /**
     * @brief Marks the event as deleted
     *
     * @param deleted
     */
    void setDeleted(bool deleted);

    /**
     * @brief Returns whether the event was removed
     */
    [[nodiscard]] bool deleted() const;

    /**
     * @brief Sets whether the event should use calendar's default reminders.
     *
     * @param useDefault
     */
    void setUseDefaultReminders(bool useDefault);

    /**
     * @brief Returns whether the event should use calendar's default reminders.
     */
    [[nodiscard]] bool useDefaultReminders() const;

    /**
     * @brief Returns ID used by Google to identify the event in calendar.
     *
     * This is not the same as UID.
     */
    [[nodiscard]] QString id() const;

    /**
     * @brief Sets event ID used by Google to identify the event in calendar.
     */
    void setId(const QString &id);

    /**
     * @brief Returns the hangout link for Google Meet.
     */
    [[nodiscard]] QString hangoutLink() const;

    /**
     * @brief Sets the hangout link for Google Meet.
     */
    void setHangoutLink(const QString &id);

private:
    class Private;
    QScopedPointer<Private> const d;
};

} // namespace KGAPI
