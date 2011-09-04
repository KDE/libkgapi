/*
    libKGoogle - Task
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


#ifndef TASK_H 
#define TASK_H 

#include "kgoogleobject.h"
#include "libkgoogle_export.h"

#ifdef WITH_KCAL
#include <kcal/todo.h>
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KCal::Todo> TodoPtr;
#else
#include <kcalcore/todo.h>
typedef KCalCore::Todo::Ptr TodoPtr;
#endif

namespace KGoogle {
  
  namespace Object {
    
#ifdef WITH_KCAL
    class LIBKGOOGLE_EXPORT Task: public KGoogleObject, public KCal::Todo
#else
    class LIBKGOOGLE_EXPORT Task: public KGoogleObject, public KCalCore::Todo
#endif
    {
      public:
	typedef QList<Task> List;

	Task();
	Task(const Task& other);
#ifdef WITH_KCAL
	Task(const KCal::Todo &other);
#else
	Task(const KCalCore::Todo &other);
#endif
	virtual ~Task();

	void setDeleted (const bool deleted);
	bool deleted();

	void setId(const QString &id);
	QString id();

      private:
	bool m_deleted;
      
    };
  } /* namespace Object */
} /* namespace KGoogle */

Q_DECLARE_METATYPE(KGoogle::Object::Task::Ptr)
Q_DECLARE_METATYPE(KGoogle::Object::Task::List)


#endif // TASK_H 
