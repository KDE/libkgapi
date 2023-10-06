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

#include <QColor>
#include <QScopedPointer>

namespace KGAPI2
{

/**
 * @brief An object that represents a Google calendar.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KGAPICALENDAR_EXPORT Calendar : public KGAPI2::Object
{
public:
    /**
     * @brief Constructor
     */
    explicit Calendar();

    /**
     * @brief Copy constructor
     */
    Calendar(const Calendar &other);

    /**
     * @brief Destructor
     */
    ~Calendar() override;

    bool operator==(const Calendar &other) const;

    /**
     * @brief Returns uID of the calendar.
     */
    [[nodiscard]] QString uid() const;

    /**
     * @brief Sets UID of the calendar.
     *
     * @param uid
     */
    void setUid(const QString &uid);

    /**
     * @brief Returns calendar title (name).
     */
    [[nodiscard]] QString title() const;

    /**
     * @brief Sets a calendar title (name).
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief Returns detailed description of the calendar.
     */
    [[nodiscard]] QString details() const;

    /**
     * @brief Sets detailed description of a calendar.
     *
     * @param details
     */
    void setDetails(const QString &details);

    /**
     * @brief Returns geographic location of the calendar.
     */
    [[nodiscard]] QString location() const;

    /**
     * @brief Sets geographic location of the calendar.
     *
     * @param location
     */
    void setLocation(const QString &location);

    /**
     * @brief Returns timezone of the calendar.
     */
    [[nodiscard]] QString timezone() const;

    /**
     * @brief Sets timezone of the calendar.
     *
     * @param timezone
     */
    void setTimezone(const QString &timezone);

    /**
     * @brief Returns whether calendar is editable or read-only.
     */
    [[nodiscard]] bool editable() const;

    /**
     * @brief Sets calendar to read-only or editable.
     *
     * @param editable
     */
    void setEditable(const bool editable);

    /**
     * @brief Sets default reminders for all new events in the calendar.
     *
     * @param reminders
     */
    void setDefaultReminders(const RemindersList &reminders);

    /**
     * @brief Adds a default reminder for all events in the calendar.
     *
     * @param reminder
     */
    void addDefaultReminer(const ReminderPtr &reminder);

    /**
     * @brief Returns default reminders for all events in the calendar.
     */
    RemindersList defaultReminders() const;

    /**
     * @brief Returns calendar background color.
     *
     * @since 2.1
     */
    [[nodiscard]] QColor backgroundColor() const;

    /**
     * @brief Sets calendar background color.
     *
     * @param color
     *
     * @since 2.1
     */
    void setBackgroundColor(const QColor &color);

    /**
     * @brief Returns calendar foreground color.
     *
     * @since 2.1
     */
    [[nodiscard]] QColor foregroundColor() const;

    /**
     * @brief Sets calendar foreground color
     *
     * @param color
     *
     * @since 2.1
     */
    void setForegroundColor(const QColor &color);

protected:
    class Private;
    QScopedPointer<Private> const d;
};

} // namespace KGAPI2
