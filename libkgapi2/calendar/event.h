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

#ifndef LIBKGAPI2_EVENT_H
#define LIBKGAPI2_EVENT_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDE/KCalCore/Event>

namespace KGAPI2
{

/**
 * @brief Represents a single event from Google Calendar
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.1
 */
class LIBKGAPI2_EXPORT Event: public KGAPI2::Object,
                             public KCalCore::Event
{
  public:

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef EventPtr Ptr;

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef QList<Event> List;

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
    explicit Event(const KCalCore::Event &event);

    /**
     * @brief Destructor
     */
    ~Event();

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

  private:
    class Private;
    Private * const d;

};

} // namepsace KGAPI

#endif // LIBKGAPI2_EVENT_H
