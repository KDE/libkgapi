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

#ifndef LIBKGAPI2_REMINDER_H
#define LIBKGAPI2_REMINDER_H

#include <libkgapi2/object.h>
#include <libkgapi2/types.h>
#include <libkgapi2/libkgapi2_export.h>

#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Incidence>

namespace KGAPI2
{

/**
 * @brief Represents a default calendar reminder.
 *
 * @author Daniel Vrátil <dvratil@redhat.com>
 * @since 0.4
 */
class LIBKGAPI2_EXPORT Reminder
{
  public:

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef ReminderPtr Ptr;

    /**
     * @deprecated Provided for backward compatibility. Will be removed in future.
     */
    KGAPI_DEPRECATED typedef RemindersList List;

    /**
     * @brief Constrcutor
     */
    explicit Reminder();

    /**
     * @brief Constructor
     *
     * @param type Type of the reminder (email, notification, etc.)
     * @param startOffset How long before the event should the reminder be shown
     */
    explicit Reminder( const KCalCore::Alarm::Type &type,
                       const KCalCore::Duration &startOffset = KCalCore::Duration( 0 ) );

    /**
     * @brief Copy constructor
     */
    Reminder( const Reminder &other );

    /**
     * @brief Destructor
     */
    virtual ~Reminder();

    /**
     * @brief Returns type of the remider
     */
    KCalCore::Alarm::Type type() const;

    /**
     * @brief Sets type of the reminder
     *
     * @param type
     */
    void setType( const KCalCore::Alarm::Type &type );

    /**
     * @brief Returns how long before the event should reminder be shown
     */
    KCalCore::Duration startOffset() const;

    /**
     * @brief Sets how long before the event should reminder be shown
     */
    void setStartOffset( const KCalCore::Duration &startOffset );

    /**
     * @brief Converts the reminder to a KCalCore::Alarm
     *
     * @param incidence An incidence on which the reminder should be applied
     * @return Returns a new KCalCore::Alarm
     */
    KCalCore::Alarm *toAlarm( KCalCore::Incidence *incidence ) const;

  private:
    class Private;
    Private *const d;
};

} // namespace KGAPI2

#endif // LIBKGAPI2_REMINDER_H
