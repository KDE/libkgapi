/*
    libKGoogle - Objects - Task List
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


#ifndef LIBKGOOGLE_OBJECTS_TASKLIST_H
#define LIBKGOOGLE_OBJECTS_TASKLIST_H

#include <libkgoogle/object.h>
#include <libkgoogle/libkgoogle_export.h>

#include <qmetatype.h>

namespace KGoogle {

  namespace Objects {

    class LIBKGOOGLE_EXPORT TaskList: public KGoogle::Object
    {
      public:
	typedef QList<TaskList> List;

	TaskList();
	virtual ~TaskList();

	void setTitle(const QString &title);
	QString title();

      private:
	QString m_title;

    };

  } /* namespace Objects */

} /* namespace KGoogle */

Q_DECLARE_METATYPE(KGoogle::Objects::TaskList)
Q_DECLARE_METATYPE(KGoogle::Objects::TaskList*)
Q_DECLARE_METATYPE(KGoogle::Objects::TaskList::List)

#endif // OBJECT_TASKLIST_H
