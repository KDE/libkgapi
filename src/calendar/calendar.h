/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LIBKGAPI2_CALENDAR_H
#define LIBKGAPI2_CALENDAR_H

#include "object.h"
#include "types.h"
#include "kgapicalendar_export.h"

#include <QtGui/QColor>

namespace KGAPI2
{

/**
 * @brief An object that represents a Google calendar.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class KGAPICALENDAR_EXPORT Calendar: public KGAPI2::Object
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
    virtual ~Calendar();

    /**
     * @brief Returns uID of the calendar.
     */
    QString uid() const;

    /**
     * @birf Sets UID of the calendar.
     *
     * @param uid
     */
    void setUid(const QString &uid);

    /**
     * @brief Returns calendar title (name).
     */
    QString title() const;

    /**
     * @brief Sets a calendar title (name).
     *
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief Returns detailed description of the calendar.
     */
    QString details() const;

    /**
     * @brief Sets detailed description of a calenar.
     *
     * @param details
     */
    void setDetails(const QString &details);

    /**
     * @brief Returns geographic location of the calendar.
     */
    QString location() const;

    /**
     * @brief Sets geographic location of the calendar.
     *
     * @param location
     */
    void setLocation(const QString &location);

    /**
     * @brief Returns timezone of the calendar.
     */
    QString timezone() const;

    /**
     * @brief Sets timezone of the calendar.
     *
     * @param timezone
     */
    void setTimezone(const QString &timezone);

    /**
     * @brief Returns whether calendar is editable or read-only.
     */
    bool editable() const;

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
     * @brief Returns defalut reminders for all events in the calendar.
     */
    RemindersList defaultReminders() const;

    /**
     * @brief Returns calendar background color.
     *
     * @since 2.1
     */
    QColor backgroundColor() const;

    /**
     * @brief Sets calendar backgrond color.
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
    QColor foregroundColor() const;

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
    Private * const d;

};

} // namespace KGAPI2

#endif // LIBKGAPI2_CALENDAR_H
