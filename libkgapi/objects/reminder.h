/*
    Copyright 2012  Dan Vratil <dan@progdan.cz>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBKGAPI_OBJECTS_REMINDER_H
#define LIBKGAPI_OBJECTS_REMINDER_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QMetaType>

#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Incidence>
typedef KCalCore::Alarm::Ptr AlarmPtr;
using namespace KCalCore;

namespace KGAPI
{

namespace Objects
{

class ReminderPrivate;

/**
 * Represents default calendar reminder.
 */
class LIBKGAPI_EXPORT_DEPRECATED Reminder
{
    public:
        typedef QSharedPointer< Reminder > Ptr;
        typedef QList< Ptr > List;

        explicit Reminder(const Alarm::Type &type, const Duration &startOffset = Duration(0));
        Reminder(const Reminder &other);
        Reminder();

        virtual ~Reminder();

        Alarm::Type type() const;

        void setType(const Alarm::Type &type);

        Duration startOffset() const;

        void setStartOffset(const Duration &startOffset);

        AlarmPtr toAlarm(Incidence *incidence) const;

    private:
        class Private;
        Private * const d;
        friend class Private;
};

} // namespace Objects

} // namespace KGAPI

#endif // LIBKGAPI_OBJECTS_REMINDER_H
