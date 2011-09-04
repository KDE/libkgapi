/*
    libKGoogle - Task List
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


#ifndef TASKLIST_H
#define TASKLIST_H

#include "kgoogleobject.h"
#include "libkgoogle_export.h"

#include <qmetatype.h>

namespace KGoogle {
  
  namespace Object {

    class LIBKGOOGLE_EXPORT TaskList: public KGoogleObject
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
  } /* namespace Object */
} /* namespace KGoogle */

Q_DECLARE_METATYPE(KGoogle::Object::TaskList)
Q_DECLARE_METATYPE(KGoogle::Object::TaskList*)
Q_DECLARE_METATYPE(KGoogle::Object::TaskList::List)

#endif // TASKLIST_H
