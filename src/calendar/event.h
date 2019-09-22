/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
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

#ifndef LIBKGAPI2_EVENT_H
#define LIBKGAPI2_EVENT_H

#include "object.h"
#include "types.h"
#include "kgapicalendar_export.h"

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
class KGAPICALENDAR_EXPORT Event: public KGAPI2::Object,
                                  public KCalendarCore::Event
{
  public:
    /**
     * @brief Constructor
     */
    Event();

    /**
     * @brief Copy constructor
     */
    Event(const Event& other);

    /**
     * @brief Copy constructor
     */
    explicit Event(const KCalendarCore::Event &event);

    /**
     * @brief Destructor
     */
    ~Event();

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
    bool deleted() const;

    /**
     * @brief Sets whether the event should use calendar's default reminders.
     *
     * @param useDefault
     */
    void setUseDefaultReminders(bool useDefault);

    /**
     * @brief Returns whether the event should use calendar's default reminders.
     */
    bool useDefaultReminders() const;

    /**
     * @brief Returns ID used by Google to identify the event in calendar.
     *
     * This is not the same as UID.
     */
    QString id() const;

    /**
     * @brief Sets event ID used by Google to identify the event in calendar.
     */
    void setId(const QString &id);

  private:
    class Private;
    QScopedPointer<Private> const d;

};

} // namespace KGAPI

#endif // LIBKGAPI2_EVENT_H
