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

#include "kgoogleobject.h"
#include "libkgoogle_export.h"

#include <kcalcore/event.h>
#include <qmetatype.h>
#include <qshareddata.h>
#include <qlist.h>

namespace KGoogle {
      
  namespace Object {
    
    class EventData;

    class LIBKGOOGLE_EXPORT Event: public KGoogleObject, public KCalCore::Event
    {
      public:
	typedef QList<Event> List;
	typedef QSharedPointer<Event> Ptr;
	
	Event();
	Event(const Event& other);
	Event(const KCalCore::Event &event);
	
	~Event();
	
	void setDeleted(const bool deleted);
	bool deleted();

	Event& operator=( const Event& other );
	
      private:
	QExplicitlySharedDataPointer<EventData> d;
      
    };
    
  } // namespace Object
  
} // namepsace KGoogle

Q_DECLARE_METATYPE(KGoogle::Object::Event::Ptr)
Q_DECLARE_METATYPE(KGoogle::Object::Event::List)


#endif // OBJECT_EVENT_H
