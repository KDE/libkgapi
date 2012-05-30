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

#ifdef WITH_KCAL
#include <KCal/Event>
#include <KCal/Person>
#include <KCal/Attendee>
#include <KCal/Alarm>
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KCal::Event> EventPtr;
typedef KCal::Attendee* AttendeePtr;
typedef KCal::Person* PersonPtr;
typedef KCal::Alarm* AlarmPtr;
#else
#include <KCalCore/Event>
#include <KCalCore/Person>
#include <KCalCore/Attendee>
#include <KCalCore/Alarm>
typedef KCalCore::Event::Ptr EventPtr;
typedef KCalCore::Attendee::Ptr AttendeePtr;
typedef KCalCore::Person::Ptr PersonPtr;
typedef KCalCore::Alarm::Ptr AlarmPtr;
#endif


namespace KGAPI
{

namespace Objects
{

class EventData;

/**
 * Information about an Event.
 */
#ifdef WITH_KCAL
class LIBKGAPI_EXPORT Event: public KGAPI::Object, public KCal::Event
#else
class LIBKGAPI_EXPORT Event: public KGAPI::Object, public KCalCore::Event
#endif
{
  public:
    typedef QList<Event> List;
    typedef QSharedPointer<Event> Ptr;

    /**
     * Constructs a new event.
     */
    Event();
    Event(const Event& other);
#ifdef WITH_KCAL
    Event(const KCal::Event &event);
#else
    Event(const KCalCore::Event &event);
#endif

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

    /**
     * Compares one event to another
     */
    Event& operator= (const Event& other);

  private:
    QSharedDataPointer<EventData> d;

};

} // namespace Objects

} // namepsace KGAPI

Q_DECLARE_METATYPE(KGAPI::Objects::Event::Ptr)
Q_DECLARE_METATYPE(KGAPI::Objects::Event::List)


#endif // LIBKGAPI_OBJECTS_EVENT_H
