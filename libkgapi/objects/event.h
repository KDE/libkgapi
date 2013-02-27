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

#ifndef LIBKGAPI_OBJECTS_EVENT_H
#define LIBKGAPI_OBJECTS_EVENT_H

#include <libkgapi/object.h>
#include <libkgapi/libkgapi_export.h>

#include <QtCore/QMetaType>
#include <QtCore/QSharedData>
#include <QtCore/QList>

#include <KDE/KCalCore/Event>
#include <KDE/KCalCore/Person>
#include <KDE/KCalCore/Attendee>
#include <KDE/KCalCore/Alarm>
typedef KCalCore::Event::Ptr EventPtr;
typedef KCalCore::Attendee::Ptr AttendeePtr;
typedef KCalCore::Person::Ptr PersonPtr;
typedef KCalCore::Alarm::Ptr AlarmPtr;

namespace KGAPI
{

namespace Objects
{

class EventPrivate;

/**
 * Information about an Event.
 */
class LIBKGAPI_EXPORT_DEPRECATED Event: public KGAPI::Object, public KCalCore::Event
{
  public:
    typedef QList<Event> List;
    typedef QSharedPointer<Event> Ptr;

    /**
     * Constructs a new event.
     */
    Event();
    Event(const Event& other);
    explicit Event(const KCalCore::Event &event);

    ~Event();

    /**
     * Tags event as deleted on the remote server.
     */
    void setDeleted(const bool deleted);

    /**
     * Returns whether the event was removed on the remote server.
     */
    bool deleted() const;

    /**
     * Sets whether the event should use calendar's default reminders.
     */
    void setUseDefaultReminders(const bool &useDefault);

    /**
     * Returns whether the event should use calendar's default reminders.
     */
    bool useDefaultReminders() const;

  private:
    class Private;
    Private * const d;
    friend class Private;

};

} // namespace Objects

} // namepsace KGAPI

Q_DECLARE_METATYPE(KGAPI::Objects::Event::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::Event::List)


#endif // LIBKGAPI_OBJECTS_EVENT_H
