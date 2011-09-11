/*
    libKGoogle - KGoogleObject - Event
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


#ifndef OBJECT_EVENT_H
#define OBJECT_EVENT_H

#include <libkgoogle/kgoogleobject.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qmetatype.h>
#include <qshareddata.h>
#include <qlist.h>
#include <kcalcore/event.h>


#ifdef WITH_KCAL
#include <kcal/event.h>
#include <kcal/person.h>
#include <kcal/attendee.h>
#include <kcal/alarm.h>
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KCal::Event> EventPtr;
typedef KCal::Attendee* AttendeePtr;
typedef KCal::Person* PersonPtr;
typedef KCal::Alarm* AlarmPtr;
#else
#include <kcalcore/event.h>
#include <kcalcore/person.h>
#include <kcalcore/attendee.h>
#include <kcalcore/alarm.h>
typedef KCalCore::Event::Ptr EventPtr;
typedef KCalCore::Attendee::Ptr AttendeePtr;
typedef KCalCore::Person::Ptr PersonPtr;
typedef KCalCore::Alarm::Ptr AlarmPtr;
#endif


namespace KGoogle {
      
  namespace Object {
    
    class EventData;

    /**
     * Information about an Event.
     */
#ifdef WITH_KCAL
    class LIBKGOOGLE_EXPORT Event: public KGoogleObject, public KCal::Event
#else
    class LIBKGOOGLE_EXPORT Event: public KGoogleObject, public KCalCore::Event
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
	 * Returns wheter the event was removed on the remote server.
	 */
	bool deleted();
	
	/**
	 * A standard-named method for KCalCore::Event::setUid()
	 */
	void setId(const QString &id);
	
	/**
	 * A standard-named method for KCalCore::Event::uid()
	 */
	QString id();

	/**
	 * Compares one event to another
	 */
	Event& operator=( const Event& other );
	
      private:
	QExplicitlySharedDataPointer<EventData> d;
      
    };
    
  } // namespace Object
  
} // namepsace KGoogle

Q_DECLARE_METATYPE(KGoogle::Object::Event::Ptr)
Q_DECLARE_METATYPE(KGoogle::Object::Event::List)


#endif // OBJECT_EVENT_H
