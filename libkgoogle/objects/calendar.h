/*
    libKGoogle - KGoogleObject - Calendar
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


#ifndef OBJECT_CALENDAR_H
#define OBJECT_CALENDAR_H

#include "kgoogleobject.h"
#include "libkgoogle_export.h"

#include <qsharedpointer.h>
#include <qmetatype.h>

#include <kdatetime.h>

namespace KGoogle {
  
  namespace Object {

    class CalendarData;    
    
    class LIBKGOOGLE_EXPORT Calendar: public KGoogleObject
    {
      
      public:
	typedef QSharedPointer<Calendar> Ptr;
	typedef QList<Calendar> List;

	Calendar();

	Calendar(const Calendar &other);

	~Calendar();
    
	QString title();
	void setTitle(const QString &title);
    
	QString color();
	void setColor(const QString &color);

	QString details();
	void setDetails(const QString &details);

	QString timezone();
	void setTimezone(const QString &timezone);

	bool editable();
	void setEditable(const bool editable);

	KDateTime created();
	void setCreated(const KDateTime &created);
     
	void setUpdated(const KDateTime &updated);
	KDateTime updated();

      protected:
	QExplicitlySharedDataPointer<CalendarData> d;

    };
    
  } // namespace Object
  
} // namespace KGoogle

Q_DECLARE_METATYPE(KGoogle::Object::Calendar)
Q_DECLARE_METATYPE(KGoogle::Object::Calendar*)
Q_DECLARE_METATYPE(KGoogle::Object::Calendar::Ptr)
Q_DECLARE_METATYPE(KGoogle::Object::Calendar::List)

#endif // OBJECT_CALENDAR_H
