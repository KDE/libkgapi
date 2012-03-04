/*
    libKGoogle - Objects - Calendar
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef LIBKGOOGLE_OBJECTS_CALENDAR_H
#define LIBKGOOGLE_OBJECTS_CALENDAR_H

#include <libkgoogle/object.h>
#include <libkgoogle/libkgoogle_export.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QMetaType>

#ifdef WITH_KCAL
#include <KDE/KCal/Alarm>
#include <KDE/KCal/Incidence>
typedef KCal::Alarm* AlarmPtr;
using namespace KCal;
#else
#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Incidence>
typedef KCalCore::Alarm::Ptr AlarmPtr;
using namespace KCalCore;
#endif

namespace KGoogle
{

namespace Objects
{

class ReminderData;

/**
 * Represents default calendar reminder.
 */
class LIBKGOOGLE_EXPORT Reminder
{
    public:
        typedef QSharedPointer< Reminder > Ptr;
        typedef QList< Ptr > List;

        Reminder();
        Reminder(const Alarm::Type &type, const Duration &startOffset = Duration(0));
        Reminder(const Reminder &other);

        ~Reminder();

        Alarm::Type type() const;

        void setType(const Alarm::Type &type);

        Duration startOffset() const;

        void setStartOffset(const Duration &startOffset);

        AlarmPtr toAlarm(Incidence *incidence) const;

    private:
        QExplicitlySharedDataPointer < ReminderData > d;
};

class CalendarData;

/**
 * Represents a calendar.
 */
class LIBKGOOGLE_EXPORT Calendar: public KGoogle::Object
{

  public:
    typedef QSharedPointer<Calendar> Ptr;
    typedef QList<Calendar> List;

    Calendar();

    Calendar(const Calendar &other);

    ~Calendar();

    /**
     * Sets UID of the calendar.
     */
    void setUid(const QString &uid);

    /**
     * Returns uID of the calendar.
     */
    QString uid() const;

    /**
     * Returns calendar title (name).
     */
    QString title() const;

    /**
     * Sets a calendar title (name).
     */
    void setTitle(const QString &title);

    /**
     * Returns detailed description of the calendar.
     */
    QString details() const;

    /**
     * Sets detailed description of a calenar.
     */
    void setDetails(const QString &details);

    /**
     * Returns geographic location of the calendar.
     */
    QString location() const;

    /**
     * Sets geographic location of the calendar.
     */
    void setLocation(const QString &location);

    /**
     * Returns timezone of the calendar.
     */
    QString timezone() const;

    /**
     * Sets timezone of the calendar.
     */
    void setTimezone(const QString &timezone);

    /**
     * Returns wheter calendar is editable or read-only.
     */
    bool editable() const;

    /**
     * Sets calendar to read-only or editable.
     */
    void setEditable(const bool editable);

    /**
     * Sets default reminders for all new events in the calendar.
     */
    void setDefaultReminders(const Reminder::List &reminders);

    /**
     * Adds a default reminder for all events in the calendar.
     */
    void addDefaultReminer(const Reminder::Ptr &reminder);

    /**
     * Returns defalut reminders for all events in the calendar.
     */
    Reminder::List defaultReminders() const;

  protected:
    QExplicitlySharedDataPointer<CalendarData> d;

};

} // namespace Objects

} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Objects::Calendar)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar*)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar::Ptr)
Q_DECLARE_METATYPE(KGoogle::Objects::Calendar::List)

#endif // LIBKGOOGLE_OBJECTS_CALENDAR_H
